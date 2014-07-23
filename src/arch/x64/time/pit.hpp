#ifndef __ANARCH_X64_PIT_HPP__
#define __ANARCH_X64_PIT_HPP__

#include <anarch/api/clock>
#include <ansa/atomic>

namespace anarch {

namespace x64 {

class Pit : public Clock {
public:
  Pit(uint16_t divisor);
  
  void Start(); // @critical
  void Stop(); // @critical
  
  virtual uint64_t GetTicks();
  virtual ansa::Rational<uint64_t> GetMicrosPerTick();
  
private:
  uint16_t divisor;
  ansa::Atomic<uint64_t> counter;
  
  static void InterruptHandler();
};

}

}

#endif
