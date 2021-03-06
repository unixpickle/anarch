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
  AssertNoncritical();
  // map Pit in the IOAPIC and the IRT
  assert(currentPit == NULL);
  
  counter = 0;
  
  // set the PIT frequency
  SetCritical(true);
  OutB(0x43, 0x34); // channel 0, lobyte/hibyte, rate generator
  OutB(0x40, (uint8_t)divisor);
  OutB(0x40, (uint8_t)(divisor >> 8));
  SetCritical(false);
  
  currentPit = this;
  Irt::GetGlobal().Set(IntVectors::Pit, (void *)&InterruptHandler);
  IOApic & ioApic = IOApicModule::GetGlobal().GetBaseIOApic();
  ioApic.MapIrq(0, IntVectors::Pit);
}

void Pit::Stop() {
  AssertNoncritical();
  assert(currentPit == this);
  
  currentPit = NULL;
  
  // set the PIT to the highest divide just for kicks
  OutB(0x43, 0x34); // channel 0, lobyte/hibyte, rate generator
  OutB(0x40, 0xff);
  OutB(0x40, 0xff);
  
  IOApic & ioApic = IOApicModule::GetGlobal().GetBaseIOApic();
  ioApic.MaskIrq(IntVectors::Pit);
  
  // don't unset the handler in the IRT because there may be another PIT
  // interrupt pending on some CPU which would cause a race condition
}

uint64_t Pit::GetTicks() {
  return counter;
}

ansa::Rational<uint64_t> Pit::GetMicrosPerTick() {
  // return (micros / second) / (ticks / second)
  return ansa::Rational<uint64_t>(1000000, 1193182UL / divisor);
}

void Pit::InterruptHandler() {
  AssertCritical();
  if (currentPit) {
    ++currentPit->counter;
  }
  LapicModule::GetGlobal().GetLapic().SendEoi();
}

}

}
