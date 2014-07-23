#include "domain-list.hpp"
#include "../acpi/acpi-module.hpp"
#include "../critical/module.hpp"
#include "../time/module.hpp"
#include "../interrupts/apic/ioapic-module.hpp"
#include "../vmm/global/global-malloc.hpp"
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
                       &ClockModule::GetGlobal());
}

void DomainList::Initialize() {
  ApicTable & apicTable = *AcpiModule::GetGlobal().GetApicTable();
  int lapicCount = apicTable.CountLapics(true);
  
  VirtualAllocator & allocator = GlobalMalloc::GetGlobal().GetAllocator();
  mainDomain = allocator.New<Domain>(lapicCount);
  assert(mainDomain != NULL);
  
  cout << "i'm going to tick/tock now!" << endl;
  Clock & clock = ClockModule::GetGlobal().GetClock();
  while (1) {
    clock.MicroSleep(500000);
    cout << "tick" << endl;
    clock.MicroSleep(500000);
    cout << "tock" << endl;
  }
}

}

}
