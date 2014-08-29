#ifndef __ANARCH_X64_PAGE_TABLE_HPP__
#define __ANARCH_X64_PAGE_TABLE_HPP__

#include <anarch/api/memory-map>
#include <anarch/api/allocator>
#include <ansa/nocopy>

namespace anarch {

namespace x64 {

/**
 * This class facilitates page table manipulation via fractal mapping. The
 * underlying memory map must be set while you manipulate the page table.
 */
class PageTable : public ansa::NoCopy {
public:
  static const VirtAddr KernelEnd = 0x8000000000UL;
  static const VirtAddr UserEnd = 0xFFFFFF8000000000UL;
  
  static int CalcDepth(size_t size);
  static uint64_t CalcMask(size_t, bool kernel,
                           const MemoryMap::Attributes &);
  
  PageTable(Allocator &, PhysAddr pml4);
  PageTable(Allocator &);
  
  void SetPml4(PhysAddr);
  PhysAddr GetPml4();
  
  void SetAllocator(Allocator &);
  Allocator & GetAllocator();
  
  /**
   * Returns `true` if the PML4 of this page table is CR3.
   * @noncritical
   */
  bool IsSet();
  
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
  bool Set(VirtAddr addr, uint64_t entry, uint64_t parentMask, int depth,
           bool * overwrote = NULL);
  
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
               uint64_t parentMask, bool * overwrote = NULL);
  
  /**
   * Perform a memory map read operation.
   * @noncritical
   */
  bool Read(PhysAddr *, MemoryMap::Attributes *, size_t *, VirtAddr);
  
  /**
   * Free all of the page table structures including the PML4.
   * @noncritical
   */
  void FreeTable(int pdptStart);
  
private:
  Allocator * allocator;
  PhysAddr pml4 = 0;
  
  /**
   * Perform a fractal-map lookup.
   * @ambicritical
   */
  static uint64_t & GetTableEntry(VirtAddr address, int depth);
  
  /**
   * Perform a fractal-map lookup for the beginning of a given table. This is
   * useful for zeroing new page tables.
   * @ambicritical
   */
  static uint64_t * GetTableStart(VirtAddr address, int depth);
  
  /**
   * Free the table at a certain [depth] that starts with [addr].
   * @critical
   */
  void FreeTableRecursive(PhysAddr table, int depth, int start = 0,
                          int end = 0x200);
};

}

}

#endif
