#include "tlb.hpp"
#include "../smp/cpu.hpp"
#include "../domains/domain-list.hpp"
#include "../interrupts/irt.hpp"
#include "../interrupts/vectors.hpp"
#include "../interrupts/apic/lapic-module.hpp"
#include <anarch/critical>
#include <anarch/new>

namespace anarch {

namespace x64 {

namespace {

Tlb globalTlb;
VirtAddr invlAddress;
size_t invlSize;
ansa::Atomic<int> ipiCount;

}

void Tlb::Invlpg(VirtAddr addr) {
  AssertCritical();
  __asm__ __volatile__("invlpg (%0)" : : "r" (addr) : "memory");
}

void Tlb::Invlpgs(VirtAddr start, size_t size) {
  AssertCritical();
  if (size > 0x200000L) {
    // at this point, it's more efficient to just clear all the caches
    if (start + size < KernelEnd) {
      __asm__("mov %%cr4, %%rax\n"
              "xor $0x80, %%rax\n"
              "mov %%rax, %%cr4\n"
              "or $0x80, %%rax\n"
              "mov %%rax, %%cr4" : : : "rax", "memory");
    } else {
      __asm__("mov %%cr3, %%rax\n"
              "mov %%rax, %%cr3"
              : : : "rax", "memory");
    }
    return;
  }
  
  // invalidate one cache entry at a time
  for (VirtAddr addr = start; addr < start + size; addr += 0x1000) {
    __asm__("invlpg (%0)" : : "r" (addr) : "memory");
  }
}

void Tlb::InitGlobal() {
  new(&globalTlb) Tlb();
}

Tlb & Tlb::GetGlobal() {
  return globalTlb;
}

void Tlb::WillSetAddressSpace(MemoryMap * map) {
  AssertCritical();
  if (!IsInitialized()) return;
  Cpu::GetCurrent().currentMap = map;
}

void Tlb::DistributeInvlpg(VirtAddr start, size_t size) {
  AssertNoncritical();
  
  if (!IsInitialized()) {
    ScopedCritical crit;
    Invlpgs(start, size);
    return;
  }
  
  ScopedLock scope(lock);
  ScopedCritical critical;
  
  Invlpgs(start, size);
  if (start < KernelEnd) {
    DistributeKernel(start, size);
  } else {
    DistributeUser(start, size);
  }
}

ansa::DepList Tlb::GetDependencies() {
  return ansa::DepList(&Irt::GetGlobal(), &LapicModule::GetGlobal(),
                       &DomainList::GetGlobal());
}

void Tlb::Initialize() {
  Irt::GetGlobal().Set(IntVectors::Invlpg, (void *)&HandleNotification);
}

void Tlb::HandleNotification() {
  AssertCritical();
  Invlpgs(invlAddress, invlSize);
  --ipiCount;
  LapicModule::GetGlobal().GetLapic().SendEoi();
}

void Tlb::DistributeKernel(VirtAddr a, size_t b) {
  invlAddress = a;
  invlSize = b;
  ipiCount = 0;
  
  // send the invlpg request to every CPU
  Cpu * current = &Cpu::GetCurrent();
  Lapic & lapic = LapicModule::GetGlobal().GetLapic();
  
  DomainList & list = DomainList::GetGlobal();
  for (int i = 0; i < list.GetCount(); i++) {
    Domain & domain = list[i];
    for (int j = 0; j < domain.GetThreadCount(); j++) {
      Cpu * cpu = &domain.GetCpu(j);
      if (cpu == current) continue;
      lapic.SendIpi(cpu->GetApicId(), IntVectors::Invlpg);
      ++ipiCount;
    }
  }
  while (ipiCount) {}
}

void Tlb::DistributeUser(VirtAddr a, size_t b) {
  Cpu * current = &Cpu::GetCurrent();
  MemoryMap * map = current->currentMap;
  
  invlAddress = a;
  invlSize = b;
  ipiCount = 0;
  
  // send the invlpg request to every CPU with our memory map
  Lapic & lapic = LapicModule::GetGlobal().GetLapic();
  
  DomainList & list = DomainList::GetGlobal();
  for (int i = 0; i < list.GetCount(); i++) {
    Domain & domain = list[i];
    for (int j = 0; j < domain.GetThreadCount(); j++) {
      Cpu * cpu = &domain.GetCpu(j);
      if (cpu->currentMap != map || cpu == current) {
        continue;
      }
      lapic.SendIpi(cpu->GetApicId(), IntVectors::Invlpg);
      ++ipiCount;
    }
  }
  while (ipiCount) {}
}

}

}
