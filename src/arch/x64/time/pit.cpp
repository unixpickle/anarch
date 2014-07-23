#include "pit.hpp"
#include "../interrupts/irt.hpp"
#include "../interrupts/vectors.hpp"
#include "../interrupts/apic/ioapic-module.hpp"
#include "../interrupts/apic/lapic-module.hpp"
#include "../common.hpp"
#include <anarch/critical>

namespace anarch {

namespace x64 {

namespace {

Pit * currentPit = NULL;

}

Pit::Pit(uint16_t _divisor) : divisor(_divisor) {
}

void Pit::Start() {
  AssertCritical();
  // map Pit in the IOAPIC and the IRT
  assert(currentPit == NULL);
  
  counter = 0;
  
  // set the PIT frequency
  OutB(0x43, 0x34); // channel 0, lobyte/hibyte, rate generator
  OutB(0x40, (uint8_t)divisor);
  OutB(0x40, (uint8_t)(divisor >> 8));
  
  currentPit = this;
  Irt::GetGlobal().Set(IntVectors::Pit, (void *)&InterruptHandler);
  IOApic & ioApic = IOApicModule::GetGlobal().GetBaseIOApic();
  ioApic.MapIrq(0, IntVectors::Pit);
}

void Pit::Stop() {
  AssertCritical();
  assert(currentPit == this);
  
  currentPit = NULL;
  
  // set the PIT to the highest divide just for kicks
  OutB(0x43, 0x34); // channel 0, lobyte/hibyte, rate generator
  OutB(0x40, 0xff);
  OutB(0x40, 0xff);
  
  // TODO: there is a race condition here when we unset the PIT in the IOAPIC
  // and in the IRT while there's still a PIT interrupt pending. Idea: never
  // actually unset the PIT from the IRT
  IOApic & ioApic = IOApicModule::GetGlobal().GetBaseIOApic();
  ioApic.MaskIrq(IntVectors::Pit);
  Irt::GetGlobal().Unset(IntVectors::Pit);
}

uint64_t Pit::GetTicks() {
  return counter;
}

ansa::Rational<uint64_t> Pit::GetMicrosPerTick() {
  // return (micros / second) / (ticks / second)
  return ansa::Rational<uint64_t>(1000000, 71590920UL / divisor);
}

void Pit::InterruptHandler() {
  AssertCritical();
  if (currentPit) {
    ++currentPit->divisor;
  }
  LapicModule::GetGlobal().GetLapic().SendEoi();
}

}

}
