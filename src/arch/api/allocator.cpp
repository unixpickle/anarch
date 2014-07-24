#include "allocator.hpp"
#include "global-map.hpp"
#include <anarch/api/panic>
#include <anarch/stddef>

namespace anarch {

bool Allocator::AllocAndMap(VirtAddr & res, PhysSize size, bool bigPages) {
  if (!GlobalMap::GetCapabilities().placementMap) {
    return AllocAndMapContiguous(res, size, bigPages);
  }
  
  GlobalMap & map = GlobalMap::GetGlobal();
  int pageSizeCount = bigPages ? GlobalMap::GetPageSizeCount() : 1;
  
  for (int i = pageSizeCount - 1; i >= 0; i--) {
    PhysSize pageSize = GlobalMap::GetPageSize(i);
    if (i != 0 && size % pageSize) continue;
    
    PhysSize pageAlign = GlobalMap::GetPageSizeAlign(i);
    PhysSize pageCount = size / pageSize;
    if (size % pageSize) pageCount++;
    
    if (!map.Reserve(res, GlobalMap::Size(pageSize, pageCount))) {
      continue;
    }
    
    GlobalMap::Size mapSize(pageSize, 1);
    GlobalMap::Attributes mapAttributes;
    
    VirtAddr dest = res;
    bool failed = false;
    
    // allocate one page at a time and map each one sequentially
    while (pageCount--) {
      PhysAddr phys;
      if (!Alloc(phys, pageSize, pageAlign)) {
        // the allocation failed, so we need to free and unmap all the previous
        // pages and then abort this attempt; we may still potentially succeed
        // with a smaller page size
        FreeAndUnmap(res, dest - res);
        failed = true;
        break;
      }
      map.MapAt(dest, phys, mapSize, mapAttributes);
      dest += (VirtAddr)pageSize;
    }
    if (!failed) return true;
  }
  
  return false;
}

void Allocator::FreeAndUnmap(VirtAddr addr, PhysSize size) {
  if (!GlobalMap::GetCapabilities().placementMap) {
    return FreeAndUnmapContiguous(addr, size);
  }
  
  GlobalMap & map = GlobalMap::GetGlobal();
  
  VirtAddr source = addr;
  PhysSize remaining = size;
  
  while (remaining) {
    PhysSize pageSize;
    PhysAddr pageAddr;
    if (!map.Read(&pageAddr, NULL, &pageSize, source)) {
      Panic("Allocator::FreeAndUnmap() - map.Read() failed");
    }
    map.Unmap(source, GlobalMap::Size(pageSize, 1));
    Free(source);
    
    source += (VirtAddr)pageSize;
    if (remaining <= pageSize) break;
    remaining -= pageSize;
  }
}

bool Allocator::AllocAndMapContiguous(VirtAddr & res, PhysSize size,
                                      bool bigPages) {
  int pageSizeCount = bigPages ? GlobalMap::GetPageSizeCount() : 1;
  GlobalMap::Attributes attributes;
  
  // try every page size that aligns with the specified size, and then try the
  // smallest page size regardless
  for (int i = pageSizeCount - 1; i >= 0; --i) {
    PhysSize pageSize = GlobalMap::GetPageSize(i);
    if (i != 0 && size % pageSize) continue;
    
    PhysSize pageAlign = GlobalMap::GetPageSizeAlign(i);
    PhysSize pageCount = size / pageSize;
    if (size % pageSize) pageCount++;
    
    // attempt to allocate
    PhysAddr physBase;
    if (!Alloc(physBase, pageCount * pageSize, pageAlign)) {
      continue;
    }
    
    // attempt to map
    GlobalMap::Size mapSize(pageSize, pageCount);
    if (!GlobalMap::GetGlobal().Map(res, physBase, mapSize, attributes)) {
      Free(physBase);
    } else {
      return true;
    }
  }
  return false;
}

void Allocator::FreeAndUnmapContiguous(VirtAddr addr, PhysSize size) {
  // compute the page size that was used
  GlobalMap & map = GlobalMap::GetGlobal();
  PhysAddr physAddr;
  PhysSize pageSize;
  if (!map.Read(&physAddr, NULL, &pageSize, addr)) {
    Panic("Allocator::FreeAndUnmapContiguous() - GlobalMap::Read() failed");
  }
  
  // unmap
  PhysSize pageCount = size / pageSize;
  if (size % pageSize) pageCount++;
  map.Unmap(addr, GlobalMap::Size(pageSize, pageCount));
  
  // free
  Free(physAddr);
}

}
