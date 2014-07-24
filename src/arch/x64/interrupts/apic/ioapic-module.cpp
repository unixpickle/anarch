#include "ioapic-module.hpp"
#include "../../common.hpp"
#include "../../acpi/acpi-module.hpp"
#include "../../critical/module.hpp"
#include "../../vmm/global/global-malloc.hpp"
#include <anarch/api/panic>
#include <anarch/critical>
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
                       &StreamModule::GetGlobal(), 
                       &CriticalModule::GetGlobal());
}

void IOApicModule::Initialize() {
  cout << "Initializing I/O APIC..." << endl;
  
  if (!AcpiModule::GetGlobal().GetApicTable()) {
    Panic("IOApicModule::Initialize() - no APIC table in ACPI");
  }
  ApicTable & table = *AcpiModule::GetGlobal().GetApicTable();
  ApicTable::IOApic * info = table.LookupIOApic(0);
  if (!info) {
    Panic("IOApicModule::Initialize() - no base I/O APIC");
  }
  
  VirtualAllocator & allocator = GlobalMalloc::GetGlobal().GetAllocator();
  baseIOApic = allocator.New<IOApic, ApicTable &>(table, info);
  assert(baseIOApic != NULL);
  
  // mask every pin before enabling I/O APIC
  for (uint32_t i = 0; i < GetBaseIOApic().GetPinCount(); i++) {
    GetBaseIOApic().MaskPin(i);
  }
  
  // enable I/O APIC on all motherboards
  SetCritical(true);
  OutB(0x22, 0x70);
  OutB(0x23, 0x01);
  SetCritical(false);
  
  cout << "Initialized I/O APIC; version=" << baseIOApic->GetVersion() << endl;
}

}

}
