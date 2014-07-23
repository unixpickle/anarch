#ifndef __ANARCH_X64_STOPPABLE_HPP__
#define __ANARCH_X64_STOPPABLE_HPP__

#include <anarch/api/clock>

namespace anarch {

namespace x64 {

class StoppableClock : public Clock {
public:
  virtual ~StoppableClock() {
  }
  
  virtual void Start() = 0; // @critical
  virtual void Stop() = 0; // @critical
};

}

}

#endif
