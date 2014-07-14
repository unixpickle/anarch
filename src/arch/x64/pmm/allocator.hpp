#ifndef __ANARCH_X64_ALLOCATOR_HPP__
#define __ANARCH_X64_ALLOCATOR_HPP__

#include "page-allocator.hpp"
#include <anarch/api/allocator>
#include <anarch/lock>
#include <analloc2>

namespace anarch {

namespace x64 {

class Allocator : public PageAllocator, public anarch::Allocator {
public:
  static const int MaxAllocators = 0x10;
  
  static void InitGlobal();
  static Allocator & GetGlobal();
  
  virtual bool AllocLower(PhysAddr &, PhysSize size, PhysSize align);
  
  // anarch::PhysicalAllocator
  virtual bool Alloc(PhysAddr &, PhysSize size, PhysSize align);
  virtual void Free(PhysAddr address);
  virtual PhysSize Used();
  virtual PhysSize Available();
  virtual PhysSize Total();
  
  // anarch::x64::PageAllocator
  virtual PhysAddr AllocPage();
  virtual void FreePage(PhysAddr p);
  
protected:
  virtual ansa::DepList GetDependencies();
  virtual void Initialize();
  
private:
  ANAlloc::FixedCluster<MaxAllocators> lower;
  ANAlloc::FixedCluster<MaxAllocators> upper;
  bool hasUpper = false;
  
  NoncriticalLock upperLock;
  NoncriticalLock lowerLock;
  
  PhysSize totalSpace = 0;
  
  static VirtAddr AllocateRaw(PhysSize size);
  void InitializeCluster(ANAlloc::MutableCluster & cluster,
                         const ANAlloc::RegionList & regs);  
};

}

}

#endif
