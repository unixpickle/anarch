#include "domain-list.hpp"
#include "../smp/startup-code.hpp"
#include "../acpi/acpi-module.hpp"
#include "../critical/module.hpp"
#include "../segments/gdt.hpp"
#include "../timer/module.hpp"
#include "../clock/module.hpp"
#include "../interrupts/idt.hpp"
#include "../interrupts/apic/ioapic-module.hpp"
#include "../interrupts/apic/lapic-module.hpp"
#include "../vmm/global/global-malloc.hpp"
#include "../vmm/global/global-map.hpp"
#include "../vmm/tlb.hpp"
#include <anarch/critical>
#include <anarch/stream>

namespace {

anarch::x64::DomainList gDomainList;

}

namespace anarch {

DomainList & DomainList::GetGlobal() {
  return gDomainList;
}

namespace x64 {

void DomainList::InitGlobal() {
  new(&gDomainList) DomainList();
}

DomainList & DomainList::GetGlobal() {
  return gDomainList;
}

int DomainList::GetCount() {
  return 1;
}

Domain & DomainList::operator[](int) {
  return *mainDomain;
}

ansa::DepList DomainList::GetDependencies() {
  return ansa::DepList(&CriticalModule::GetGlobal(),
                       &IOApicModule::GetGlobal(),
                       &LapicModule::GetGlobal(),
                       &ClockModule::GetGlobal(),
                       &Gdt::GetGlobal(),
                       &StreamModule::GetGlobal());
}

ansa::DepList DomainList::GetSuperDependencies() {
  return ansa::DepList(&Tlb::GetGlobal(), &TimerModule::GetGlobal());
}

void DomainList::Initialize() {
  ApicTable & apicTable = *AcpiModule::GetGlobal().GetApicTable();
  int lapicCount = apicTable.CountLapics(true);
  
  VirtualAllocator & allocator = GlobalMalloc::GetGlobal().GetAllocator();
  mainDomain = allocator.New<Domain>(lapicCount);
  assert(mainDomain != NULL);
  
  mainDomain->InitNewCpu();
  
  StartCpus();
}

void DomainList::StartCpus() {
  // configure the bootstrap code
  StartupCode startupCode(&CpuEntrance);
  
  // iterate through CPUs
  ApicTable & apicTable = *AcpiModule::GetGlobal().GetApicTable();
  if (!apicTable.CanIterate()) return;
  ApicTable::Iterator iterator = apicTable.GetIterator();
  
  do {
    if (iterator.GetType() == ApicTable::TypeLapic) {
      void * data = iterator.GetData();
      ApicTable::LocalApic * info = (ApicTable::LocalApic *)data;
      
      startupCode.ResetStartupStack();
      BootstrapCpu((uint32_t)info->apicId);
    } else if (iterator.GetType() == ApicTable::TypeX2Apic) {
      void * data = iterator.GetData();
      ApicTable::LocalApic2 * info = (ApicTable::LocalApic2 *)data;
      
      startupCode.ResetStartupStack();
      BootstrapCpu(info->x2apicId);
    }
  } while (iterator.Next());
}

void DomainList::BootstrapCpu(uint32_t apicId) {
  ScopedCritical critical;
  Lapic & lapic = LapicModule::GetGlobal().GetLapic();
  Clock & clock = ClockModule::GetGlobal().GetClock();
  if (apicId == lapic.GetId()) return;
  cout << "Starting CPU " << apicId << "...";
  
  lapic.ClearErrors();
  lapic.SendIpi(apicId, 0, 5, 1, 1);
  
  SetCritical(false);
  clock.Sleep(1);
  SetCritical(true);
  
  lapic.SendIpi(apicId, 0, 5, 0, 1);
  
  initFlag = 0;
  // send STARTUP IPI (and resend if needed, as it is on some AMD systems)
  for (int j = 0; j < 2; j++) {
    lapic.ClearErrors();
    
    // send vector 5 for address 0x5000
    lapic.SendIpi(apicId, 5, 6, 1, 0);

    // wait a maximum of 200ms before sending another IPI or failing
    for (int i = 0; i < 20; i++) {
      SetCritical(false);
      clock.MicroSleep(10000);
      SetCritical(true);
      
      ScopedLock scope(initLock);
      if (initFlag) return;
    }
  }
  
  cerr << " [FAIL]" << endl;
}

void DomainList::CpuEntrance() {
  DomainList & list = DomainList::GetGlobal();
  list.initLock.Seize();
  
  // set various CPU structures
  Idt::GetGlobal().Set();
  GlobalMap::GetGlobal().Set();
  Gdt::GetGlobal().Set();
  
  // configure LAPIC
  Lapic & lapic = LapicModule::GetGlobal().GetLapic();
  lapic.Enable();
  lapic.SetDefaults();
  
  // create actual CPU object, which sets up TSS and GS automatically
  SetCritical(false);
  int idx = list.mainDomain->InitNewCpu();
  Cpu & cpu = list.mainDomain->GetCpu(idx);
  SetCritical(true);
  
  // transition to CPU stack and call CpuStall; I use a `call` instead of a
  // `jmp` to preserve 16-byte alignment in compliance with the System V ABI.
  __asm__ __volatile__("mov %%rax, %%rsp\ncall *%%rbx"
                       : : "a" (cpu.GetStackTop()), "b" (&CpuStall));
}

void DomainList::CpuStall() {
  cout << " [OK]" << endl;
  DomainList & list = DomainList::GetGlobal();
  list.initFlag = true;
  list.initLock.Release();
  
  SetCritical(false);
  
  while (1) {
    __asm__ __volatile__("hlt");
  }
}

}

}
