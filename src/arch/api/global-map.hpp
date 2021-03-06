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
  static size_t GetPageSize(int);
  static size_t GetPageSizeAlign(int);
  static Capabilities GetCapabilities();
};

}

#endif
