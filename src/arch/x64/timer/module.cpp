#include "module.hpp"
#include "../domains/domain-list.hpp"
#include "../clock/module.hpp"
#include "../interrupts/vectors.hpp"
#include "../interrupts/irt.hpp"
#include "../interrupts/apic/lapic-module.hpp"
#include <anarch/critical>
#include <anarch/stream>

namespace anarch {

namespace x64 {

namespace {

TimerModule gTimerModule;

}

void TimerModule::InitGlobal() {
  new(&gTimerModule) TimerModule();
}

TimerModule & TimerModule::GetGlobal() {
  return gTimerModule;
}

ansa::DepList TimerModule::GetDependencies() {
  return ansa::DepList(&DomainList::GetGlobal());
}

void TimerModule::Initialize() {
  ScopedCritical critical;
  
  cout << "Initializing LAPIC timer subsystem...";
  
  Lapic & lapic = LapicModule::GetGlobal().GetLapic();
  Irt::GetGlobal().Set(IntVectors::Calibrate, (void *)&CalibrateMethod);
  
  DomainList & domains = DomainList::GetGlobal();
  for (int i = 0; i < domains.GetCount(); i++) {
    Domain & domain = domains[i];
    for (int i = 0; i < domain.GetThreadCount(); i++) {
      Cpu & cpu = domain.GetCpu(i);
      if (&cpu == &Cpu::GetCurrent()) continue;
      lapic.SendIpi(cpu.GetApicId(), IntVectors::Calibrate);
    }
  }
  
  CalibrateMethod();
  
  // wait until every CPU is calibrated
  for (int i = 0; i < domains.GetCount(); i++) {
    Domain & domain = domains[i];
    for (int i = 0; i < domain.GetThreadCount(); i++) {
      LapicTimer & timer = domain.GetCpu(i).GetLapicTimer();
      while (!timer.calibrated) {
        __asm__ __volatile__("pause");
      }
    }
  }
  
  Irt::GetGlobal().Unset(IntVectors::Calibrate);
  Irt::GetGlobal().Set(IntVectors::LapicTimer,
                       (void *)&LapicTimer::GeneralTimerCallback);
  
  cout << " [OK]" << endl;
}

void TimerModule::CalibrateMethod() {
  AssertCritical();
  LapicTimer & timer = Cpu::GetCurrent().GetLapicTimer();
  
  Lapic & lapic = LapicModule::GetGlobal().GetLapic();
  if (lapic.IsInService(IntVectors::Calibrate)) {
    lapic.SendEoi();
  }
  lapic.WriteReg(Lapic::RegLvtTimer, 0xff);
  lapic.WriteReg(Lapic::RegTimerInitCount, 0xffffffff);
  
  SetCritical(false);
  ClockModule::GetGlobal().GetClock().MicroSleep(500000);
  SetCritical(true);
  
  uint64_t value = lapic.ReadReg(Lapic::RegTimerCurrCount);
  lapic.ClearTimeout();
  
  assert(value > 0);
  
  uint64_t ticksPerSec = (uint64_t)(0xffffffff - value) * 2;
  timer.ticksPerMicro = ansa::Rational<uint64_t>(ticksPerSec, 1000000);
  timer.calibrated = true;
}

}

}
