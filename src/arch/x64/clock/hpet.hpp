#ifndef __ANARCH_X64_HPET_HPP__
#define __ANARCH_X64_HPET_HPP__

#include "stoppable.hpp"
#include "../acpi/hpet-table.hpp"
#include <anarch/easy-map>

namespace anarch {

namespace x64 {

class Hpet : public StoppableClock {
public:
  Hpet(const HpetTable & info); // @noncritical
  
  // destructor not shown, but it's also @noncritical
  
  virtual void Start(); // @critical
  virtual void Stop(); // @critical
  
  virtual uint64_t GetTicks(); // @ambicritical
  virtual ansa::Rational<uint64_t> GetMicrosPerTick(); // @ambicritical
  
private:
  EasyMap map;
  ansa::Rational<uint64_t> microsPerTick;
};

}

}

#endif
