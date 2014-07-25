#ifndef __ANARCH_X64_LAPIC_TIMER_HPP__
#define __ANARCH_X64_LAPIC_TIMER_HPP__

#include <anarch/api/timer>
#include <ansa/atomic>

namespace anarch {

namespace x64 {

class TimerModule;

class LapicTimer : public anarch::Timer {
public:
  LapicTimer(); // zero initialize ticksPerMicro
  
  // anarch::Timer
  virtual ansa::Rational<uint64_t> GetTicksPerMicro();
  virtual void SetTimeout(uint64_t ticks, void (* func)());
  virtual void ClearTimeout();
  virtual void WaitTimeout() ANSA_NORETURN;
  
protected:
  ansa::Rational<uint64_t> ticksPerMicro;
  ansa::Atomic<bool> calibrated;
  
  static void GeneralTimerCallback();
  
  friend class TimerModule;
  
private:
  bool isExpecting = false;
  void * callbackFunction;
};

}

}

#endif
