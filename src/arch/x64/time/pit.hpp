#ifndef __ANARCH_X64_PIT_HPP__
#define __ANARCH_X64_PIT_HPP__

#include "stoppable.hpp"
#include <ansa/atomic>

namespace anarch {

namespace x64 {

class Pit : public StoppableClock {
public:
  Pit(uint16_t divisor);
  
  virtual void Start(); // @critical
  virtual void Stop(); // @critical
  
  virtual uint64_t GetTicks(); // @ambicritical
  virtual ansa::Rational<uint64_t> GetMicrosPerTick(); // @ambicritical
  
private:
  uint16_t divisor;
  ansa::Atomic<uint64_t> counter;
  
  static void InterruptHandler();
};

}

}

#endif
