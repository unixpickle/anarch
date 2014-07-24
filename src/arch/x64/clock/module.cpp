#include "module.hpp"
#include "hpet.hpp"
#include "pit.hpp"
#include "../acpi/acpi-module.hpp"
#include "../interrupts/irt.hpp"
#include "../interrupts/apic/ioapic-module.hpp"
#include "../interrupts/apic/lapic-module.hpp"
#include "../vmm/global/global-malloc.hpp"
#include <anarch/critical>
#include <anarch/stream>
#include <anarch/new>

namespace anarch {

namespace x64 {

namespace {

ClockModule gClockModule;

}

void ClockModule::InitGlobal() {
  new(&gClockModule) ClockModule();
}

ClockModule & ClockModule::GetGlobal() {
  return gClockModule;
}

void ClockModule::TryTsc() {
  AssertNoncritical();
}

bool ClockModule::IsUsingTsc() {
  return isTsc;
}

Clock & ClockModule::GetClock() {
  assert(clock != NULL);
  return *clock;
}

ansa::DepList ClockModule::GetDependencies() {
  return ansa::DepList(&Irt::GetGlobal(), &IOApicModule::GetGlobal(),
                       &LapicModule::GetGlobal(), &GlobalMalloc::GetGlobal(),
                       &AcpiModule::GetGlobal(), &StreamModule::GetGlobal());
}

void ClockModule::Initialize() {
  VirtualAllocator & allocator = GlobalMalloc::GetGlobal().GetAllocator();
  HpetTable * hpetInfo = AcpiModule::GetGlobal().GetHpetTable();
  
  if (hpetInfo) {
    clock = allocator.New<Hpet, const HpetTable &>(*hpetInfo);
    cout << "HPET clock initialized" << endl;
  } else {
    clock = allocator.New<Pit, uint16_t>(1193); // ~1000Hz
    cout << "PIT clock initialized" << endl;
  }
  clock->Start();
}

}

}
