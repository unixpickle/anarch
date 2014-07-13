#ifndef __ANARCH_API_ALLOCATOR_HPP__
#define __ANARCH_API_ALLOCATOR_HPP__

#include <anarch/types>

namespace anarch {

class Allocator {
public:
  /**
   * Allocate aligned physical memory.
   * @noncritical
   */
  virtual PhysAddr Alloc(PhysSize size, PhysSize align) = 0;
  
  /**
   * Free physical memory.
   * @noncritical
   */
  virtual void Free(PhysAddr address) = 0;
  
  /**
   * Get the amount of physical memory in use by the system.
   * @noncritical
   */
  virtual PhysSize Used() = 0;
  
  /**
   * Get the amount of unused physical memory available to the system.
   * @noncritical
   */
  virtual PhysSize Available() = 0;
  
  /**
   * Get the amount of total physical memory on the system.
   * @noncritical
   */
  virtual PhysSize Total() = 0;
};

}

#endif
