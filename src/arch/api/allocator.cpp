#include "allocator.hpp"
#include "global-map.hpp"

namespace anarch {

VirtAddr Allocator::AllocAndMap(PhysSize size) {
  GlobalMap & map = GlobalMap::GetGlobal();
  Allocator & alloc = map.GetPageAllocator();
  
  // TODO: here, attempt to allocate the memory with bigger page sizes if the
  // requested amount of memory *is* a bigger page size
  
  PhysSize pageSize = GlobalMap::GetPageSize(0);
  PhysSize pageAlign = GlobalMap::GetPageSizeAlign(0);
  PhysSize pageCount = size / pageSize + (size % pageSize ? 1 : 0);
  VirtAddr reserved;
  if (!map.Reserve(reserved, GlobalMap::Size(pageSize, pageCount))) {
    Panic("Allocator::AllocAndMap() - reserve failed");
  }
  
  GlobalMap::Size mapSize(pageSize, 1);
  GlobalMap::Attributes mapAttributes;
  
  // allocate one page at a time and map each one sequentially
  VirtAddr dest = reserved;
  while (pageCount--) {
    PhysAddr phys;
    if (!alloc.Alloc(phys, pageSize, pageAlign)) {
      Panic("BuddyAllocator::AllocFromGlobalMap() - allocation failed");
    }
    map.MapAt(dest, phys, mapSize, mapAttributes);
    dest += pageSize;
  }

  return reserved;
}

}
