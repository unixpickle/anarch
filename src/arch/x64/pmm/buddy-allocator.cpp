#include "buddy-allocator.hpp"
#include "../vmm/global-map.hpp"
#include <anarch/critical>

namespace anarch {

namespace x64 {

BuddyAllocator::BuddyAllocator(const RegionList & regions) {
  // initialize the buddy allocator piece by piece like a boss
  for (int i = 0; i < regions.GetLowerRegions().GetCount(); i++) {
    totalSpace += (size_t)regions.GetLowerRegions()[i].GetSize();
  }
  for (int i = 0; i < regions.GetUpperRegions().GetCount(); i++) {
    totalSpace += (size_t)regions.GetUpperRegions()[i].GetSize();
  }

  if (regions.GetLowerRegions().GetCount()) {
    InitializeCluster(lower, regions.GetLowerRegions());
    hasLower = true;
  }
  if (regions.GetUpperRegions().GetCount()) {
    InitializeCluster(upper, regions.GetUpperRegions());
    hasUpper = true;
  }
}

virtual void Reserve(PhysAddr lastAddr) {
  if (hasLower) {
    lower.Reserve(0, (ANAlloc::UInt)lastAddr);
  }
  if (hasUpper) {
    upper.Reserve(0, (ANAlloc::UInt)lastAddr);
  }
}

bool BuddyAllocator::AllocLower(PhysAddr & addr, PhysSize size,
                                PhysSize align) {
  AssertNoncritical();
  if (!hasLower) return false;
  
  ScopedLock scope(lowerLock);
  
  ANAlloc::UInt result;
  if (!lower.Align((ANAlloc::UInt)size, (ANAlloc::UInt)align, result)) {
    return false;
  }
  addr = (PhysAddr)result;
  return true;
}

bool BuddyAllocator::Alloc(PhysAddr & addr, PhysSize size, PhysSize align) {
  AssertNoncritical();
  if (hasUpper) {
    ScopedLock scope(upperLock);
    ANAlloc::UInt result;
    if (upper.Align((ANAlloc::UInt)size, (ANAlloc::UInt)align, result)) {
      addr = (PhysAddr)result;
      return true;
    }
  }
  return AllocLower(addr, size, align);
}

void BuddyAllocator::Free(PhysAddr address) {
  AssertNoncritical();
  if (address >= 0x100000000L) {
    ScopedLock scope(upperLock);
    upper.Free((ANAlloc::UInt)address);
  } else {
    ScopedLock scope(lowerLock);
    lower.Free((ANAlloc::UInt)address);
  }
}

PhysSize BuddyAllocator::Used() {
  AssertNoncritical();
  return totalSpace - Available();
}

PhysSize BuddyAllocator::Available() {
  AssertNoncritical();
  PhysSize available = 0;
  if (hasUpper) {
    ScopedLock scope(upperLock);
    available += (PhysSize)upper.GetFreeSize();
  }
  if (hasLower) {
    ScopedLock scope(lowerLock);
    available += (PhysSize)lower.GetFreeSize();
  }
  return available;
}

PhysSize BuddyAllocator::Total() {
  AssertNoncritical();
  return totalSpace;
}

VirtAddr BuddyAllocator::AllocFromGlobalMap(PhysSize size) {
  // perform a quick and dirty memory allocation
  GlobalMap & map = GlobalMap::GetGlobal();
  Allocator & alloc = map.GetPageAllocator();
  
  PhysSize pageSize = size >= 0x200000 ? 0x200000 : 0x1000;
  PhysSize pageCount = size / pageSize + (size % pageSize ? 1 : 0);
  VirtAddr reserved = map.Reserve(GlobalMap::Size(pageSize, pageCount));
  VirtAddr dest = reserved;

  GlobalMap::Size mapSize(pageSize, 1);
  GlobalMap::Attributes mapAttributes;
  if (GlobalMap::SupportsNX()) {
    attributes.executable = false;
  }
  
  // allocate a page at a time and map each one sequentially
  while (pageCount--) {
    PhysAddr phys;
    if (!alloc.Alloc(phys, pageSize, pageSize)) {
      Panic("BuddyAllocator::AllocFromGlobalMap() - allocation failed");
    }
    map.MapAt(dest, phys, mapSize, mapAttributes);
    dest += pageSize;
  }

  return reserved;

}

void BuddyAllocator::InitializeCluster(ANAlloc::MutableCluster & cluster,
                                       const ANAlloc::RegionList & regs) {
  ANAlloc::FixedDescList<MaxAllocators> descs;

  // create the descriptions
  ANAlloc::Layout layout(descs, regs, 12, 0x1000000, 0x1000);
  layout.Run();

  // create the allocators and their trees
  ANAlloc::ClusterBuilder<ANAlloc::BBTree> builder(descs, cluster, 12);
  ANAlloc::UInt space = builder.RequiredSpace();
  
  Allocator & alloc = GlobalMap::GetGlobal().GetPageAllocator();
  VirtAddr newAddress = alloc.AllocAndMap((PhysSize)space);
  builder.CreateAllocators((uint8_t *)newAddress);
}

}

}
