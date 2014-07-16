#include "../page-table.hpp"

namespace anarch {

namespace x64 {

class FreeFinder {
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