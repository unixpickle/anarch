#ifndef __ANARCH_X64_CLOCK_MODULE_HPP__
#define __ANARCH_X64_CLOCK_MODULE_HPP__

#include <anarch/api/clock-module>
#include <anarch/stddef>
#include "stoppable.hpp"

namespace anarch {

namespace x64 {

class ClockModule : public anarch::ClockModule {
public:
  static void InitGlobal();
  static ClockModule & GetGlobal();
  
  // try to setup the Time Stamp Counter as a system clock
  virtual void TryTsc(); // @noncritical
  virtual bool IsUsingTsc(); // @ambicritical
  
  // anarch::ClockModule
  virtual Clock & GetClock();
  
protected:
  virtual ansa::DepList GetDependencies();
  virtual void Initialize();
  
private:
  StoppableClock * clock = NULL;
  bool isTsc = false;
};

}

}

#endif
