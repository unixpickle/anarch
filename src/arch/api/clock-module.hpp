#ifndef __ANARCH_API_CLOCK_MODULE_HPP__
#define __ANARCH_API_CLOCK_MODULE_HPP__

#include <ansa/module>

namespace anarch {

class Clock;

class ClockModule : public ansa::Module {
public:
  static ClockModule & GetGlobal();
  
  virtual Clock & GetClock() = 0; // @ambicritical
};

}

#endif
