#include "ioapic-module.hpp"
#include "../../vmm/global/global-malloc.hpp"
#include "../../acpi/acpi-module.hpp"
#include <anarch/api/panic>
#include <anarch/stream>
#include <anarch/new>

namespace anarch {

namespace x64 {

namespace {

IOApicModule gIOApicModule;

}

void IOApicModule::InitGlobal() {
  new(&gIOApicModule) IOApicModule();
}

IOApicModule & IOApicModule::GetGlobal() {
  return gIOApicModule;
}

IOApic & IOApicModule::GetBaseIOApic() {
  return *baseIOApic;
}

ansa::DepList IOApicModule::GetDependencies() {
  return ansa::DepList(&AcpiModule::GetGlobal(), &GlobalMalloc::GetGlobal(),
                       &StreamModule::GetGlobal());
}

void IOApicModule::Initialize() {
  if (!AcpiModule::GetGlobal().GetApicTable()) {
    Panic("IOApicModule::Initialize() - no APIC table in ACPI");
  }
  ApicTable & table = *AcpiModule::GetGlobal().GetApicTable();
  ApicTable::IOApic * info = table.LookupIOApic(0);
  if (!info) {
    Panic("IOApicModule::Initialize() - no base I/O APIC");
  }
  
  VirtualAllocator & allocator = GlobalMalloc::GetGlobal().GetAllocator();
  baseIOApic = allocator.New<IOApic>(table, info);
  assert(baseIOApic != NULL);
  
  cout << "Initialized I/O APIC; version=" << baseIOApic->GetVersion() << endl;
}

}

}
