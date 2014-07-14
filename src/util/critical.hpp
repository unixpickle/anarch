#ifndef __ANARCH_UTIL_CRITICAL_HPP__
#define __ANARCH_UTIL_CRITICAL_HPP__

#include "assert.hpp"
#include "../arch/api/critical.hpp"

#define AssertCritical() assert(::anarch::IgnoreCriticality() || \
  ::anarch::GetCritical());
#define AssertNoncritical() assert(::anarch::IgnoreCriticality() || \
  !::anarch::GetCritical());

namespace anarch {

class ScopedCritical {
public:
  ScopedCritical(); // @ambicritical (x)
  ~ScopedCritical(); // @critical -> @ambicritical (x)
  
protected:
  bool wasCritical;
};

}

#endif
