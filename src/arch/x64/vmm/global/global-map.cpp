#include "global-map.hpp"
#include "map-setup.hpp"
#include "../scoped-scratch.hpp"
#include "../tlb.hpp"
#include "../../pmm/step-allocator.hpp"
#include "../../pmm/buddy-allocator.hpp"
#include <anarch/x64/init>
#include <anarch/api/panic>
#include <anarch/critical>
#include <anarch/stream>
#include <anarch/assert>
#include <anarch/new>
#include <ansa/math>

namespace anarch {

GlobalMap & GlobalMap::GetGlobal() {
  return x64::GlobalMap::GetGlobal();
}

int GlobalMap::GetPageSizeCount() {
  return 2;
}

PhysSize GlobalMap::GetPageSize(int idx) {
  assert(idx >= 0 && idx < 2);
  if (idx == 0) return 0x1000;
  return 0x200000;
}

PhysSize GlobalMap::GetPageSizeAlign(int idx) {
  return GetPageSize(idx);
}

bool GlobalMap::SupportsReadAddress() {
  return true;
}

bool GlobalMap::SupportsReadAttributes() {
  return true;
}

bool GlobalMap::SupportsReadSize() {
  return true;
}

bool GlobalMap::SupportsReserveAt() {
  return true;
}

bool GlobalMap::SupportsReserve() {
  return true;
}

bool GlobalMap::SupportsMapAt() {
  return true;
}

bool GlobalMap::SupportsNX() {
  return true;
}

bool GlobalMap::SupportsRO() {
  return true;
}

bool GlobalMap::SupportsUncached() {
  return true;
}

namespace x64 {

namespace {

GlobalMap gGlobalMap;

}

void GlobalMap::InitGlobal() {
  new(&gGlobalMap) GlobalMap();
}

GlobalMap & GlobalMap::GetGlobal() {
  return gGlobalMap;
}

Scratch & GlobalMap::GetScratch() {
  return *scratch;
}

PageTable & GlobalMap::GetPageTable() {
  return *pageTable;
}

FreeFinder & GlobalMap::GetFreeFinder() {
  return *freeFinder;
}

Allocator & GlobalMap::GetPageAllocator() {
  return *pageAllocator;
}

PhysAddr GlobalMap::GetPDPT() {
  return pdpt;
}

void GlobalMap::Set() {
  AssertCritical();
  TLB::GetGlobal().WillSetAddressSpace(*this);
  __asm__("mov %0, %%cr3" : : "r" (GetPageTable().GetPML4()));
}

bool GlobalMap::Read(PhysAddr * physOut, Attributes * attrOut,
                     PhysSize * sizeOut, VirtAddr addr) {
  AssertNoncritical();
  ScopedLock scope(lock);
  
  uint64_t entry;
  int depth = GetPageTable().Walk(addr, entry, sizeOut);
  if (depth < 0) return false;
  if (!entry) return false;
  
  assert(depth >= 2);
  
  if (physOut) {
    if (depth == 3) {
      *physOut = entry & 0x7ffffffffffff000UL;
    } else {
      *physOut = entry & 0x7fffffffffe00000UL;
    }
  }
  
  if (attrOut) {
    attrOut->executable = (entry & (1UL << 63)) != 0;
    attrOut->writable = (entry & 2) != 0;
  
    // TODO: set cachable flag properly here
    attrOut->cachable = true;
  }
  
  return true;
}

bool GlobalMap::Map(VirtAddr & addr, PhysAddr phys, Size size,
                    const Attributes & attributes) {
  AssertNoncritical();
  ScopedLock scope(lock);
  
  if (!GetFreeFinder().Alloc(addr, size.Bytes(), size.pageCount)) {
    return false;
  }
  
  cout << "GlobalMap::Map() - giving out " << addr << endl;
  
  uint64_t mask = PageTable::CalcMask(size.pageSize, true, attributes);
  GetPageTable().SetList(addr, (uint64_t)phys | mask, size, 3);
  
  return true;
}

void GlobalMap::MapAt(VirtAddr addr, PhysAddr phys, Size size,
                      const Attributes & attributes) {
  AssertNoncritical();
  ScopedLock scope(lock);
  
  uint64_t mask = PageTable::CalcMask(size.pageSize, true, attributes);
  GetPageTable().SetList(addr, (uint64_t)phys | mask, size, 3);

  // we may have overwritten something
  TLB::GetGlobal().DistributeInvlpg(addr, size.Bytes());
}

void GlobalMap::Unmap(VirtAddr addr, Size size) {
  cout << "GlobalMap::Unmap(" << addr << ", " << size.pageSize <<
    ", " << size.pageCount << ")" << endl;
  
  AssertNoncritical();
  ScopedLock scope(lock);
  
  VirtAddr next = addr;
  for (PhysSize i = 0; i < size.pageCount; i++) {
    if (!GetPageTable().Unset(next)) {
      Panic("GlobalMap::Unmap() - Unset() failed");
    }
    next += size.pageSize;
  }
  
  GetFreeFinder().Free(addr, size.Bytes());
  
  // we should invalidate the old memory before releasing the lock since new
  // mappings might not invalidate it.
  TLB::GetGlobal().DistributeInvlpg(addr, size.Bytes());
}

bool GlobalMap::Reserve(VirtAddr & addr, Size size) {
  AssertNoncritical();
  ScopedLock scope(lock);
  
  if (!GetFreeFinder().Alloc(addr, size.Bytes(), size.pageSize)) {
    return false;
  }
  
  uint64_t entry = size.pageSize | (size.pageSize == 0x1000 ? 0 : 0x80);
  GetPageTable().SetList(addr, entry, size, 3);
  
  return true;
}

void GlobalMap::ReserveAt(VirtAddr addr, Size size) {
  AssertNoncritical();
  ScopedLock scope(lock);
  
  GetFreeFinder().Reserve(addr, size.Bytes());
  
  uint64_t entry = size.pageSize | (size.pageSize == 0x1000 ? 0 : 0x80);
  GetPageTable().SetList(addr, entry, size, 3);
}

ansa::DepList GlobalMap::GetDependencies() {
  return ansa::DepList(&StreamModule::GetGlobal());
}

void GlobalMap::Initialize() {
  cout << "Initializing x64::GlobalMap" << endl;
  
  MapSetup setup;
  
  setup.GenerateMap();
  setup.GenerateScratch();
  scratch = setup.GetScratch();
  
  cout << " - generating global PageTable" << endl;
  
  setup.GeneratePageTable();
  pageTable = setup.GetPageTable();
  
  cout << " - generating FreeFinder" << endl;
  
  setup.GenerateFreeFinder();
  freeFinder = setup.GetFreeFinder();
  
  {
    ScopedCritical critical;
    Set();
  }
  
  cout << " - generating BuddyAllocator" << endl;
  
  setup.GenerateBuddyAllocator();
  pageAllocator = setup.GetBuddyAllocator();
}

}

}
