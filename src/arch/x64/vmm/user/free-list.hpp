#ifndef __ANARCH_X64_FREE_LIST_HPP__
#define __ANARCH_X64_FREE_LIST_HPP__

#include <anarch/types>
#include <anarch/stddef>

namespace anarch {

namespace x64 {

class FreeList {
public:
  struct Region {
    VirtAddr start;
    size_t size;
    Region * next = NULL;
  
    Region(VirtAddr, size_t);
    VirtAddr End();
    bool CanHold(size_t pageSize, size_t pageCount);
    bool IsAdjacentBehind(VirtAddr addr);
    bool Contains(Region & reg);
    bool IsBehind(VirtAddr addr);
    bool IsFilledBy(Region & reg);
    bool IsEndedBy(Region & reg);
    bool IsStartedBy(Region & reg);
  };
  
  ~FreeList(); // @noncritical
  
  /**
   * Allocate a contiguous, aligned region at a specified address.
   * @noncritical
   */
  bool AllocAt(VirtAddr start, size_t pageSize, size_t pageCount);

  /**
   * Allocate a contiguous, aligned region of virtual memory.
   * @noncritical
   */
  VirtAddr Alloc(size_t pageSize, size_t pageCount);

  /**
   * Free a contiguous region of virtual memory.
   * @noncritical
   */
  void Free(VirtAddr addr, size_t pageSize, size_t pageCount);

protected:
  Region * first = NULL;

  static Region * AllocRegion(VirtAddr, size_t);
  static void FreeRegion(Region * reg);

  void AllocInRegion(Region * reg, Region * last, Region & requested);
};

}

}

#endif
