#ifndef __ANARCH_X64_STEP_ALLOCATOR_HPP__
#define __ANARCH_X64_STEP_ALLOCATOR_HPP__

#include <anarch/api/allocator>

namespace anarch {

namespace x64 {

/**
 * A simple allocator for temporary use while the OS is initializing. This
 * allocator is not appropriate for permanent use because it does not support
 * any operation other than simple allocation.
 */
class StepAllocator : public Allocator {
public:
  StepAllocator(PhysAddr start);
  
  virtual PhysAddr GetLastAddress();
  
  // anarch::Allocator
  virtual bool Alloc(PhysAddr & addr, PhysSize size, PhysSize align);
  virtual void Free(PhysAddr); // unsupported
  virtual bool Owns(PhysAddr); // unsupported
  virtual PhysSize Used(); // unsupported
  virtual PhysSize Available(); // unsupported
  virtual PhysSize Total(); // unsupported
  
private:
  PhysAddr lastAddr;
};

}

}

#endif
