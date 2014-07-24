#include "acpi-module.hpp"
#include "acpi-pointer.hpp"
#include "../vmm/global/global-malloc.hpp"
#include <anarch/phys-copy>
#include <anarch/assert>
#include <anarch/stream>

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
  return ansa::DepList(&GlobalMalloc::GetGlobal(),
                       &StreamModule::GetGlobal());
}

void AcpiModule::Initialize() {
  cout << "Initializing ACPI subsystem...";
  AcpiPointer * pointer = AcpiPointer::Find();
  assert(pointer != NULL);
  
  root = pointer->GenerateRoot();
  assert(root != NULL);
  
  VirtualAllocator & allocator = GlobalMalloc::GetGlobal().GetAllocator();
  
  int apicIndex = GetRoot().FindTable("APIC");
  if (apicIndex >= 0) {
    apicTable = allocator.New<ApicTable>(GetRoot().GetTable(apicIndex));
    assert(apicTable != NULL);
  }
  int hpetIndex = GetRoot().FindTable("HPET");
  if (hpetIndex >= 0) {
    hpetTable = allocator.New<HpetTable>();
    PhysCopy((void *)&hpetTable, GetRoot().GetTable(hpetIndex),
             sizeof(hpetTable));
  }
  cout << " [OK]" << endl;
}

}

}
