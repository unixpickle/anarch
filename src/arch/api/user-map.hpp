#ifndef __ANARCH_API_USER_MAP_HPP__
#define __ANARCH_API_USER_MAP_HPP__

#include "memory-map.hpp"
#include <anarch/stddef>

namespace anarch {

class UserMap : public MemoryMap {
public:
  // both @noncritical
  static UserMap & New();
  virtual void Delete() = 0;
  
  // all @ambicritical
  static int GetPageSizeCount();
  static PhysSize GetPageSize(int);
  static PhysSize GetPageSizeAlign(int);
  static Capabilities GetCapabilities();
  
  /**
   * Copy user memory to kernel memory. A normal memcpy() cannot be used
   * because a task's kernel thread can access memory that the normal thread
   * cannot (namely, kernel memory). This method should generate an artificial
   * page fault if it detects any sort of illegal access.
   * @noncritical
   */
  virtual void CopyToKernel(void * dest, VirtAddr start, size_t size) = 0;
  
  /**
   * Like CopyToKernel(), but copies the other way.
   * @noncritical
   */
  virtual void CopyFromKernel(VirtAddr dest, void * start, size_t size) = 0;
  
protected:
  virtual ~UserMap() {}
};

}

#endif
