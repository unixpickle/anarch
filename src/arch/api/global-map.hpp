#ifndef __ANARCH_API_GLOBAL_MAP_HPP__
#define __ANARCH_API_GLOBAL_MAP_HPP__

#include <ansa/module>
#include "memory-map.hpp"

namespace anarch {

class GlobalMap : public ansa::Module, public MemoryMap {
public:
  // all @ambicritical
  static GlobalMap & GetGlobal();
  static int GetPageSizeCount();
  static PhysSize GetPageSize(int);
  static PhysSize GetPageSizeAlign(int);
  static bool SupportsReadAddress();
  static bool SupportsReadAttributes();
  static bool SupportsReadSize();
  static bool SupportsReserveAt();
  static bool SupportsReserve();
  static bool SupportsMapAt();
  static bool SupportsNX();
  static bool SupportsRO();
  static bool SupportsUncached();
};

}

#endif
