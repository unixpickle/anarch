#ifndef __ANARCH_X64_BUDDY_ALLOCATOR_HPP__
#define __ANARCH_X64_BUDDY_ALLOCATOR_HPP__

#include <anarch/api/allocator>
#include <anarch/x64/region-list>
#include <anarch/lock>

namespace anarch {

namespace x64 {

/**
 * This allocator is meant for use throughout the lifecycle of the operating
 * system's execution. It provides efficient allocation and deallocation.
 */
class BuddyAllocator : public Allocator {
public:
  BuddyAllocator(const RegionList & regions, Allocator & tempAllocator);
  
  virtual void Reserve(PhysAddr lastAddr);
  
  virtual bool AllocLower(PhysAddr & addr, PhysSize size, PhysSize align);
  
  // anarch::Allocator
  virtual bool Alloc(PhysAddr & result, PhysSize size, PhysSize align);
  virtual void Free(PhysAddr address);
  virtual bool Owns(PhysAddr address);
  virtual PhysSize Used();
  virtual PhysSize Available();
  virtual PhysSize Total();
  
private:
  static const int MaxAllocators = 0x10;
  
  ANAlloc::FixedCluster<MaxAllocators> lower;
  ANAlloc::FixedCluster<MaxAllocators> upper;
  bool hasUpper = false;
  bool hasLower = false;
  
  NoncriticalLock upperLock;
  NoncriticalLock lowerLock;
  
  PhysSize totalSpace = 0;
  
  static void InitializeCluster(ANAlloc::MutableCluster & cluster,
                                const ANAlloc::RegionList & regs,
                                Allocator & tempAllocator);
};

}

}

#endif
