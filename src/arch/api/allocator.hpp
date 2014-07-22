#ifndef __ANARCH_API_ALLOCATOR_HPP__
#define __ANARCH_API_ALLOCATOR_HPP__

#include <anarch/types>
#include <ansa/nocopy>

namespace anarch {

class Allocator : public ansa::NoCopy {
public:
  /**
   * Allocate aligned physical memory.
   * @noncritical
   */
  virtual bool Alloc(PhysAddr & result, PhysSize size, PhysSize align) = 0;
  
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
  
  /**
   * Allocate physical memory and map it into contiguous virtual memory. This
   * makes no guarantees about which page size it will use. If the allocation
   * fails, this function will Panic() the kernel.
   */
  virtual VirtAddr AllocAndMap(PhysSize size);
  
  /**
   * Do the inverse of AllocAndMap(). This method looks up each page, unmaps
   * it, frees it, and moves on to the next one.
   */
  virtual void FreeAndUnmap(VirtAddr addr, PhysSize size);
};

}

#endif
