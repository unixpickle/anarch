#include "module.hpp"
#include "pit.hpp"
#include "../interrupts/irt.hpp"
#include "../interrupts/apic/ioapic-module.hpp"
#include "../interrupts/apic/lapic-module.hpp"
#include "../vmm/global/global-malloc.hpp"
#include <anarch/critical>
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
                       &LapicModule::GetGlobal(), &GlobalMalloc::GetGlobal());
}

void ClockModule::Initialize() {
  // TODO: potentially use the HPET if it is available
  
  VirtualAllocator & allocator = GlobalMalloc::GetGlobal().GetAllocator();
  clock = allocator.New<Pit, uint16_t>(1193); // ~1000Hz
  
  clock->Start();
}

}

}
