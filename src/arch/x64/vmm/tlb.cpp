#include "tlb.hpp"
#include "scratch.hpp"
#include <anarch/critical>
#include <anarch/new>

namespace anarch {

namespace x64 {

namespace {

TLB globalTLB;

}

void TLB::Invlpg(VirtAddr addr) {
  AssertCritical();
  __asm__ __volatile__("invlpg (%0)" : : "r" (addr) : "memory");
}

void TLB::Invlpgs(VirtAddr start, PhysSize size) {
  AssertCritical();
  if (size > 0x200000L) {
    // at this point, it's more efficient to just clear all the caches
    if (start + size <= Scratch::StartAddr) {
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

void TLB::InitGlobal() {
  new(&globalTLB) TLB();
}

TLB & TLB::GetGlobal() {
  return globalTLB;
}

void TLB::WillSetAddressSpace(MemoryMap &) {
  // TODO: something here
  AssertCritical();
}

void TLB::DistributeInvlpg(VirtAddr start, PhysSize size) {
  ScopedCritical scope;
  Invlpgs(start, size);
}
  
ansa::DepList TLB::GetDependencies() {
  // TODO: something here
  return ansa::DepList();
}

void TLB::Initialize() {
  // TODO: this
}

}

}
