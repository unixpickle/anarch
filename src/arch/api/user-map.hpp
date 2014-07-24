#ifndef __ANARCH_API_USER_MAP_HPP__
#define __ANARCH_API_USER_MAP_HPP__

#include "memory-map.hpp"

namespace anarch {

class UserMap : public MemoryMap {
public:
  // both @noncritical
  static UserMap & New();
  virtual void Delete();
  
  // all @ambicritical
  static int GetPageSizeCount();
  static PhysSize GetPageSize(int);
  static PhysSize GetPageSizeAlign(int);
  static Capabilities GetCapabilities();
  
protected:
  virtual ~UserMap() {}
};

}

#endif
