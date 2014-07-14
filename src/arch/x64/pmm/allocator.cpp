#include "allocator.hpp"
#include "region-list.hpp"
#include "../vmm/global-map.hpp"
#include "../vmm/scratch.hpp"
#include <anarch/critical>
#include <anarch/stream>
#include <anarch/lock>
#include <anarch/new>

namespace anarch {

namespace x64 {

static Allocator allocator;

void Allocator::InitGlobal() {
  new(&allocator) Allocator();
}

Allocator & Allocator::GetGlobal() { 
  return allocator;
}

bool Allocator::AllocLower(PhysAddr & addr, PhysSize size, PhysSize align) {
  AssertNoncritical();
  ScopedLock scope(lowerLock);
  
  return lower.Align((ANAlloc::UInt)size, (ANAlloc::UInt)align, addr);
}

bool Allocator::Alloc(PhysAddr & addr, PhysSize size, PhysSize align) {
  AssertNoncritical();
  
  if (!hasUpper) return AllocLower(size, align);
  
  upperLock.Seize();
  bool res = upper.Align((ANAlloc::UInt)size, (ANAlloc::UInt)align, addr);
  upperLock.Release();
  if (res) return true;
  
  return AllocLower(size, align);
}

void Allocator::Free(PhysAddr address) {
  AssertNoncritical();
  if (address < 0x100000000L) {
    ScopedLock scope(lowerLock);
    lower.Free((ANAlloc::UInt)address);
  } else {
    ScopedLock scope(upperLock);
    upper.Free((ANAlloc::UInt)address);
  }
}

PhysSize Allocator::Used() {
  AssertNoncritical();
  return totalSpace - Available();
}

PhysSize Allocator::Available() {
  AssertNoncritical();
  
  PhysSize available = 0;
  
  lowerLock.Seize();  
  available += (PhysSize)lower.GetFreeSize();
  lowerLock.Release();
  
  if (hasUpper) {
    ScopedLock scope(upperLock);
    available += (PhysSize)upper.GetFreeSize();
  }
  
  return available;
}

PhysSize Allocator::Total() {
  AssertNoncritical();
  return totalSpace;
}

PhysAddr Allocator::AllocPage() {
  AssertNoncritical();
  return Alloc(0x1000, 0x1000);
}

void Allocator::FreePage(PhysAddr p) {
  AssertNoncritical();
  Free(p);
}

// PROTECTED //

ansa::DepList Allocator::GetDependencies() {
  return ansa::DepList(&GlobalMap::GetGlobal(), &Scratch::GetGlobal(),
                       &RegionList::GetGlobal(), &StreamModule::GetGlobal());
}

void Allocator::Initialize() {
  cout << "Initializing physical allocator..." << endl;
  
  RegionList & regions = RegionList::GetGlobal();
  for (int i = 0; i < regions.GetLowerRegions().GetCount(); i++) {
    totalSpace += (size_t)regions.GetLowerRegions()[i].GetSize();
  }
  for (int i = 0; i < regions.GetUpperRegions().GetCount(); i++) {
    totalSpace += (size_t)regions.GetUpperRegions()[i].GetSize();
  }
  
  InitializeCluster(lower, regions.GetLowerRegions());
  if (regions.GetUpperRegions().GetCount()) {
    InitializeCluster(upper, regions.GetUpperRegions());
    hasUpper = true;
  }
  
  PageAllocator & theAllocator = *GlobalMap::GetGlobal().allocator;
  StepAllocator & alloc = static_cast<StepAllocator &>(theAllocator);
  
  lower.Reserve(0, (ANAlloc::UInt)alloc.GetLastAddress());
  if (hasUpper) {
    upper.Reserve(0, (ANAlloc::UInt)alloc.GetLastAddress());
  }

  GlobalMap::GetGlobal().allocator = this;
}

// PRIVATE //

VirtAddr Allocator::AllocateRaw(PhysSize size) {
  PageAllocator & theAllocator = *GlobalMap::GetGlobal().allocator;
  StepAllocator & alloc = static_cast<StepAllocator &>(theAllocator);
  
  GlobalMap & map = GlobalMap::GetGlobal();
  
  PhysSize pageSize = size >= 0x200000 ? 0x200000 : 0x1000;
  PhysSize pageCount = size / pageSize + (size % pageSize ? 1 : 0);
  VirtAddr reserved = map.Reserve(GlobalMap::Size(pageSize, pageCount));
  VirtAddr dest = reserved;
  
  while (pageCount--) {
    PhysAddr page = alloc.AllocSize(pageSize);
    map.MapAt(dest, GlobalMap::MapInfo(pageSize, 1, page, false));
    dest += pageSize;
  }
  
  return reserved;
}

void Allocator::InitializeCluster(ANAlloc::MutableCluster & cluster,
                                  const ANAlloc::RegionList & regs) {
  ANAlloc::FixedDescList<MaxAllocators> descs;
  
  // create the descriptions
  ANAlloc::Layout layout(descs, regs, 12, 0x1000000, 0x1000);
  layout.Run();
  
  // create the allocators and their trees
  ANAlloc::ClusterBuilder<ANAlloc::BBTree> builder(descs, cluster, 12);
  ANAlloc::UInt space = builder.RequiredSpace();
  VirtAddr newAddress = AllocateRaw((PhysSize)space);
  builder.CreateAllocators((uint8_t *)newAddress);
}

}

}
