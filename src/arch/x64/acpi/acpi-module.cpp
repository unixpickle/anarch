#include "acpi-module.hpp"
#include "acpi-pointer.hpp"
#include "../vmm/global/global-malloc.hpp"
#include <anarch/assert>

namespace anarch {

namespace x64 {

namespace {

AcpiModule gAcpiModule;

}

void AcpiModule::InitGlobal() {
  new(&gAcpiModule) AcpiModule();
}

AcpiModule & AcpiModule::GetGlobal() {
  return gAcpiModule;
}

AcpiRoot & AcpiModule::GetRoot() {
  return *root;
}

ApicTable * AcpiModule::GetApicTable() {
  return apicTable;
}

ansa::DepList AcpiModule::GetDependencies() {
  return ansa::DepList(&GlobalMalloc::GetGlobal());
}

void AcpiModule::Initialize() {
  AcpiPointer * pointer = AcpiPointer::Find();
  assert(pointer != NULL);
  
  root = pointer->GenerateRoot();
  assert(root != NULL);
  
  VirtualAllocator & allocator = GlobalMalloc::GetGlobal().GetAllocator();
  
  int apicIndex = GetRoot().FindTable("APIC");
  if (apicIndex >= 0) {
    apicTable = allocator.New<ApicTable>(GetRoot().GetTable(apicIndex));
  }
}

}

}
