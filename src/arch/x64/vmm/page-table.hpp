#ifndef __ANARCH_X64_PAGE_TABLE_HPP__
#define __ANARCH_X64_PAGE_TABLE_HPP__

#include <anarch/api/memory-map>
#include <anarch/api/allocator>
#include "scratch.hpp"

namespace anarch {

namespace x64 {

/**
 * This class facilitates page table manipulation.
 */
class PageTable {
public:
  static int CalcDepth(PhysSize size);
  static uint64_t CalcMask(PhysSize, bool kernel,
                           const MemoryMap::Attributes &);
  
  PageTable(Allocator &, Scratch &, PhysAddr pml4);
  PageTable(Allocator &, Scratch &);
  
  void SetPML4(PhysAddr);
  PhysAddr GetPML4();
  
  void SetAllocator(Allocator &);
  Allocator & GetAllocator();
  
  /**
   * @return The depth of the entry found (0-3 inclusive) or -1 if not mapped.
   * This may return depth 3 with entry=0.
   * @noncritical -> @critical -> @noncritical
   */
  int Walk(VirtAddr addr, uint64_t & entry, size_t * size);
  
  /**
   * Set a raw entry in the page table.
   * @param addr The start of the page
   * @param entry The entry to set, flags included
   * @param parentMask All parent table entries will be ORed with this
   * @param depth The depth of the entry to create; 0-3 inclusive.
   * @return true on success; false will only be returned in the case where a
   * large page has been mapped and you try to Set() a smaller piece of that
   * large page at a deeper point.
   * @noncritical -> @critical -> @noncritical
   */
  bool Set(VirtAddr addr, uint64_t entry, uint64_t parentMask, int depth);
  
  /**
   * Unset the page table entry beginning at `addr`.
   * @return The only time Unset() can fail is if you try to unmap a piece of
   * a larger page (i.e. you try to allocate a 2MB region but passed an address
   * that is not 2MB aligned).
   * @noncritical -> @critical -> @noncritical
   */
  bool Unset(VirtAddr addr);
  
  /**
   * Set a bunch of entries at once.
   * @noncritical
   */
  void SetList(VirtAddr virt, uint64_t phys, MemoryMap::Size size,
               uint64_t parentMask);
  
  /**
   * Free all of the page table structures including the PML4.
   * @noncritical
   */
  void FreeTable(int pdptStart);
  
private:
  Allocator * allocator;
  Scratch & scratch;
  PhysAddr pml4 = 0;
  
  /**
   * @critical
   */
  void FreeTableRecursive(PhysAddr table, int depth, int start);
};

}

}

#endif
