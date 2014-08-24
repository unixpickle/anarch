#include "user-map.hpp"
#include "../tlb.hpp"
#include "../scoped-scratch.hpp"
#include "../global/global-map.hpp"
#include <anarch/api/page-delegate>
#include <anarch/api/domain>
#include <anarch/api/panic>
#include <anarch/critical>
#include <anarch/assert>
#include <ansa/cstring>

namespace anarch {

UserMap & UserMap::New() {
  UserMap * map = Domain::GetCurrent().New<x64::UserMap>();
  return *map;
}

int UserMap::GetPageSizeCount() {
  return 2;
}

size_t UserMap::GetPageSize(int idx) {
  assert(idx >= 0 && idx < 3);
  if (idx == 0) return 0x1000;
  return 0x200000;
}

size_t UserMap::GetPageSizeAlign(int idx) {
  return GetPageSize(idx);
}

UserMap::Capabilities UserMap::GetCapabilities() {
  Capabilities caps;
  caps.placementReserve = true;
  caps.placementMap = true;
  caps.executableFlag = true;
  caps.writableFlag = true;
  caps.cachableFlag = false; // TODO: figure this one out
  return caps;
}

namespace x64 {

UserMap::UserMap() : table(Domain::GetCurrent().GetAllocator(),
                           GlobalMap::GetGlobal().GetScratch()) {
  // allocate the PML4
  PhysAddr pml4;
  if (!table.GetAllocator().Alloc(pml4, 0x1000, 0x1000)) {
    Panic("UserMap::UserMap() - failed to allocate PML4");
  }
  table.SetPml4(pml4);
  
  // push both canonical regions to the free list
  freeList.Free(SpaceStart, 0x1000L, 0x7F8000000L);
  freeList.Free(0xFFFF800000000000L, 0x1000L, 0x800000000L);
  
  ScopedCritical critical;
  TypedScratch<uint64_t> tableContent(table.GetScratch(), pml4);
  tableContent[0] = GlobalMap::GetGlobal().GetPdpt() | 3;
  for (int i = 1; i < 0x200; ++i) {
    tableContent[i] = 0;
  }
}

UserMap::~UserMap() {
  table.FreeTable(1);
}

PageTable & UserMap::GetPageTable() {
  return table;
}

void UserMap::Set() {
  AssertCritical();
  Tlb::GetGlobal().WillSetAddressSpace(*this);
  __asm__("mov %0, %%cr3" : : "r" (table.GetPml4()));
}

bool UserMap::Read(PhysAddr * addrOut, Attributes * attr, size_t * size,
                   VirtAddr addr) {
  AssertNoncritical();
  ScopedLock scope(lock);
  return GetPageTable().Read(addrOut, attr, size, addr);
}

bool UserMap::Map(VirtAddr & addr, PhysAddr phys, Size size,
                  const Attributes & attributes) {
  AssertNoncritical();
  ScopedLock scope(lock);
  addr = freeList.Alloc(size.pageSize, size.pageCount);
  if (!addr) return false;
  assert(addr >= SpaceStart);
  
  uint64_t mask = PageTable::CalcMask(size.pageSize, false, attributes);
  GetPageTable().SetList(addr, (uint64_t)phys | mask, size, 7);
  return true;
}

void UserMap::MapAt(VirtAddr addr, PhysAddr phys, Size size,
                    const Attributes & attributes) {
  AssertNoncritical();
  ScopedLock scope(lock);
  
  uint64_t mask = PageTable::CalcMask(size.pageSize, false, attributes);
  GetPageTable().SetList(addr, (uint64_t)phys | mask, size, 7);
  DistInvlpg(addr, size.Bytes());
}

void UserMap::Unmap(VirtAddr addr, Size size) {
  AssertNoncritical();
  ScopedLock scope(lock);
  
  VirtAddr next = addr;
  for (size_t i = 0; i < size.pageCount; i++) {
    if (!GetPageTable().Unset(next)) {
      Panic("UserMap::Unmap() - Unset() failed");
    }
    next += size.pageSize;
  }
  
  freeList.Free(addr, size.pageSize, size.pageCount);
  DistInvlpg(addr, size.Bytes());
}

void UserMap::UnmapAndReserve(VirtAddr addr, Size size) {
  AssertNoncritical();
  ScopedLock scope(lock);
  
  VirtAddr next = addr;
  for (size_t i = 0; i < size.pageCount; i++) {
    if (!GetPageTable().Unset(next)) {
      Panic("UserMap::Unmap() - Unset() failed");
    }
    next += size.pageSize;
  }
  
  DistInvlpg(addr, size.Bytes());
}

bool UserMap::Reserve(VirtAddr & addr, Size size) {
  AssertNoncritical();
  ScopedLock scope(lock);
  addr = freeList.Alloc(size.pageSize, size.pageCount);
  return addr != 0;
}

void UserMap::ReserveAt(VirtAddr addr, Size size) {
  assert(addr >= SpaceStart);
  assert(addr + size.Bytes() >= SpaceStart);
  ScopedLock scope(lock);
  if (!freeList.AllocAt(addr, size.pageSize, size.pageCount)) {
    Panic("UserMap::ReserveAt() - failed");
  }
}

void UserMap::Unreserve(VirtAddr addr, Size size) {
  AssertNoncritical();
  ScopedLock scope(lock);
  freeList.Free(addr, size.pageSize, size.pageCount);
}

void UserMap::Rereserve(VirtAddr addr, Size oldSize, size_t newPageSize) {
  // there isn't really anything to do here, but i'll still do some assertions
  AssertNoncritical();
  assert(oldSize.Bytes() % newPageSize == 0);
  assert(addr % newPageSize == 0);
  
  // when are assert()s aren't compiled, this fixes warnings
  (void)addr;
  (void)oldSize;
  (void)newPageSize;
}

void UserMap::Delete() {
  Domain::GetCurrent().Delete(this);
}

void UserMap::CopyToKernel(void * dest, VirtAddr start, size_t size) {
  if (start + size < start || start < SpaceStart) {
    if (!GetGlobalPageDelegate()) {
      Panic("UserMap::CopyToKernel() - page fault with no delegate");
    }
    GetGlobalPageDelegate()(start, false);
    return;
  }

  ansa::Memcpy(dest, (void *)start, size);
}

void UserMap::CopyFromKernel(VirtAddr dest, void * start, size_t size) {
  if (dest + size < dest || dest < SpaceStart) {
    if (!GetGlobalPageDelegate()) {
      Panic("UserMap::CopyFromKernel() - page fault with no delegate");
    }
    GetGlobalPageDelegate()(dest, true);
    return;
  }
  
  ansa::Memcpy((void *)dest, start, size);
}

void UserMap::DistInvlpg(VirtAddr start, size_t size) {
  Tlb::GetGlobal().DistributeUserInvlpg(start, size, *this);
}

}

}
