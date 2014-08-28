#ifndef __ANARCH_X64_FREE_FINDER_HPP__
#define __ANARCH_X64_FREE_FINDER_HPP__

#include "../page-table.hpp"
#include <ansa/nocopy>

namespace anarch {

namespace x64 {

class FreeFinder : public ansa::NoCopy {
public:
  FreeFinder(PageTable & pt);
  
  bool Alloc(VirtAddr &, size_t size, size_t align);
  void Free(VirtAddr, size_t size);
  
  void Reserve(VirtAddr addr, size_t size);
  
protected:
  PageTable & pageTable;
  
  VirtAddr freeStart = 0;
  size_t freeSize = 0;
  
  void UpdateFreeRegion();
  bool CanAllocate(size_t size, size_t align);
};

}

}

#endif
