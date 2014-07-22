#include "domain-list.hpp"
#include "../acpi/acpi-module.hpp"
#include "../interrupts/apic/ioapic-module.hpp"
#include "../critical/module.hpp"
#include "../vmm/global/global-malloc.hpp"

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
                       &IOApicModule::GetGlobal());
}

void DomainList::Initialize() {
  ApicTable & apicTable = *AcpiModule::GetGlobal().GetApicTable();
  int lapicCount = apicTable.CountLapics(true);
  
  VirtualAllocator & allocator = GlobalMalloc::GetGlobal().GetAllocator();
  mainDomain = allocator.New<Domain>(lapicCount);
  assert(mainDomain != NULL);
}

}

}
