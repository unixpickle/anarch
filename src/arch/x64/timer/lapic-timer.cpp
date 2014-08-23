#include "lapic-timer.hpp"
#include "../smp/cpu.hpp"
#include "../interrupts/apic/lapic-module.hpp"
#include "../interrupts/vectors.hpp"
#include <anarch/critical>

namespace anarch {

namespace x64 {

void LapicTimer::GeneralTimerCallback() {
  AssertCritical();
  LapicTimer & timer = Cpu::GetCurrent().GetLapicTimer();
  Lapic & lapic = LapicModule::GetGlobal().GetLapic();
  if (!timer.isExpecting) {
    lapic.SendEoi();
    return;
  }
  
  // prevent race conditions with a reset
  if (lapic.IsTimerRunning()) {
    lapic.SendEoi();
    if (lapic.IsInService(IntVectors::LapicTimer)
        || lapic.IsRequested(IntVectors::LapicTimer)) {
      return;
    }
  } else {
    lapic.SendEoi();
  }
  
  timer.isExpecting = false;
  timer.callbackFunction(timer.callbackArg);
}

LapicTimer::LapicTimer() : ticksPerMicro(1, 0), calibrated(false) {
}

ansa::Rational<uint64_t> LapicTimer::GetTicksPerMicro() {
  return ticksPerMicro;
}

void LapicTimer::SetTimeout(uint64_t ticks, void (* func)()) {
  SetTimeout(ticks, (void (*)(void *))func, NULL);
}

void LapicTimer::SetTimeout(uint64_t ticks, void (* fn)(void *), void * arg) {
  AssertCritical();
  assert(ticks < 0x100000000); // TODO: support bigger times
  Lapic & lapic = LapicModule::GetGlobal().GetLapic();
  lapic.SetTimeout(IntVectors::LapicTimer, (uint32_t)ticks);
  isExpecting = true;
  callbackFunction = fn;
  callbackArg = arg;
}

void LapicTimer::ClearTimeout() {
  AssertCritical();
  Lapic & lapic = LapicModule::GetGlobal().GetLapic();
  lapic.ClearTimeout();
  isExpecting = false;
}

void LapicTimer::WaitTimeout() {
  AssertCritical();
  SetCritical(false);
  while (1) {
    __asm__ __volatile__("hlt");
  }
}

}

}