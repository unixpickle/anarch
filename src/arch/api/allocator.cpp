#include "allocator.hpp"
#include "global-map.hpp"
#include <anarch/api/panic>
#include <anarch/stddef>

namespace anarch {

VirtAddr Allocator::AllocAndMap(PhysSize size) {
  GlobalMap & map = GlobalMap::GetGlobal();
  
  if (!GlobalMap::SupportsMapAt() || !GlobalMap::SupportsReserve()) {
    // TODO: here, allocate a contiguous buffer and then map it.
    Panic("Allocator::AllocAndMap() currently unsupported");
  }
  
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
    if (!Alloc(phys, pageSize, pageAlign)) {
      Panic("Allocator::AllocAndMap() - allocation failed");
    }
    map.MapAt(dest, phys, mapSize, mapAttributes);
    dest += (VirtAddr)pageSize;
  }

  return reserved;
}

void Allocator::FreeAndUnmap(VirtAddr addr, PhysSize size) {
  GlobalMap & map = GlobalMap::GetGlobal();
  
  if (!GlobalMap::SupportsReadAddress()) {
    Panic("Allocator::FreeAndUnmap() - GlobalMap::Read() [address] required");
  }
  
  if (!GlobalMap::SupportsReadSize()) {
    Panic("Allocator::FreeAndUnmap() - GlobalMap::Read() [size] required");
  }
  
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
    
    remaining -= pageSize;
    source += (VirtAddr)pageSize;
  }
}

}
