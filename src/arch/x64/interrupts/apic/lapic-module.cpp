#include "lapic-module.hpp"
#include "../../vmm/global/global-map.hpp"
#include "../../acpi/acpi-module.hpp"
#include "../../common.hpp"
#include <anarch/critical>
#include <anarch/stream>
#include <anarch/new>

namespace anarch {

namespace x64 {

namespace {

LapicModule gLapicModule;

}

void LapicModule::InitGlobal() {
  new(&gLapicModule) LapicModule();
}

LapicModule & LapicModule::GetGlobal() {
  return gLapicModule;
}

Lapic & LapicModule::GetLapic() {
  AssertCritical();
    
  uint32_t ecx;
  CpuID(1, NULL, &ecx, NULL);
  if (ecx & (1 << 21)) {
    return *((X2Apic *)x2apicData);
  } else {
    return *((XApic *)xapicData);
  }
}

ansa::DepList LapicModule::GetDependencies() {
  return ansa::DepList(&GlobalMap::GetGlobal(), &StreamModule::GetGlobal());
}

void LapicModule::Initialize() {
  cout << "Initializing LAPIC module...";
  
  ApicTable * table = AcpiModule::GetGlobal().GetApicTable();
  assert(table != NULL);
  
  new(xapicData) XApic((PhysAddr)table->GetHeader().lapicAddr);
  new(x2apicData) X2Apic();
  
  SetCritical(true);
  GetLapic().Enable();
  GetLapic().SetDefaults();
  SetCritical(false);
  
  cout << " [OK]" << endl;
}

}

}
