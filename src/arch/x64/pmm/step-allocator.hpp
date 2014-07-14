#ifndef __ANARCH_X64_STEP_ALLOCATOR_HPP__
#define __ANARCH_X64_STEP_ALLOCATOR_HPP__

#include "page-allocator.hpp"

namespace anarch {

namespace x64 {

class StepAllocator : public PageAllocator {
public:
  StepAllocator(PhysAddr start);
  
  // PageAllocator
  virtual PhysAddr AllocPage();
  virtual void FreePage(PhysAddr p);
  
  // specialized methods
  virtual PhysAddr AllocSize(PhysSize size);
  PhysAddr GetLastAddress();

private:
  PhysAddr lastAddr;
};

}

}

#endif
