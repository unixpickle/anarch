#ifndef __ANARCH_X64_FREE_FINDER_HPP__
#define __ANARCH_X64_FREE_FINDER_HPP__

#include "../page-table.hpp"
#include <ansa/nocopy>

namespace anarch {

namespace x64 {

class FreeFinder : public ansa::NoCopy {
public:
  FreeFinder(PageTable & pt);
  
  bool Alloc(VirtAddr &, PhysSize size, PhysSize align);
  void Free(VirtAddr, PhysSize size);
  
  void Reserve(VirtAddr addr, PhysSize size);
  
protected:
  PageTable & pageTable;
  
  VirtAddr freeStart = 0;
  PhysSize freeSize = 0;
  
  void UpdateFreeRegion();
  bool CanAllocate(PhysSize size, PhysSize align);
};

}

}

#endif
