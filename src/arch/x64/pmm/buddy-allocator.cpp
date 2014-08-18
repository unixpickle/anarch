#include "buddy-allocator.hpp"
#include <anarch/api/panic>
#include <anarch/critical>

namespace anarch {

namespace x64 {

BuddyAllocator::BuddyAllocator(const RegionList & regions,
                               Allocator & tempAllocator) {
  // initialize the buddy allocator piece by piece like a boss
  for (int i = 0; i < regions.GetLowerRegions().GetCount(); i++) {
    totalSpace += (size_t)regions.GetLowerRegions()[i].GetSize();
  }
  for (int i = 0; i < regions.GetUpperRegions().GetCount(); i++) {
    totalSpace += (size_t)regions.GetUpperRegions()[i].GetSize();
  }

  if (regions.GetLowerRegions().GetCount()) {
    InitializeCluster(lower, regions.GetLowerRegions(), tempAllocator);
    hasLower = true;
  }
  if (regions.GetUpperRegions().GetCount()) {
    InitializeCluster(upper, regions.GetUpperRegions(), tempAllocator);
    hasUpper = true;
  }
}

void BuddyAllocator::Reserve(PhysAddr lastAddr) {
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

bool BuddyAllocator::Owns(PhysAddr address) {
  if (hasUpper) {
    if (upper.OwnsAddress((ANAlloc::UInt)address)) {
      return true;
    }
  }
  if (hasLower) {
    return lower.OwnsAddress((ANAlloc::UInt)address);
  }
  return false;
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

void BuddyAllocator::InitializeCluster(ANAlloc::MutableCluster & cluster,
                                       const ANAlloc::RegionList & regs,
                                       Allocator & tempAllocator) {
  ANAlloc::FixedDescList<MaxAllocators> descs;

  // create the descriptions
  ANAlloc::Layout layout(descs, regs, 12, 0x1000000, 0x1000);
  layout.Run();

  // create the allocators and their trees
  ANAlloc::ClusterBuilder<ANAlloc::BBTree> builder(descs, cluster, 12);
  ANAlloc::UInt space = builder.RequiredSpace();
  
  VirtAddr newAddress;
  if (!tempAllocator.AllocAndMap(newAddress, (PhysSize)space)) {
    Panic("BuddyAllocator::InitializeCluster() - allocation failed");
  }
  builder.CreateAllocators((uint8_t *)newAddress);
}

}

}
