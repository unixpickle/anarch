#include "global-map.hpp"
#include "../pmm/step-allocator.hpp"
#include "../pmm/buddy-allocator.hpp"
#include <anarch/x64/init>
#include <anarch/assert>
#include <anarch/new>

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

Allocator & GlobalMap::GetPageAllocator() {
  return *pageAllocator;
}

void GlobalMap::Set() {
  // TODO: this
}

bool GlobalMap::Map(VirtAddr &, PhysAddr, Size, const Attributes &) {
  // TODO: this
  return false;
}

void GlobalMap::MapAt(VirtAddr, PhysAddr, Size, const Attributes &) {
  // TODO: this
}

void GlobalMap::Unmap(VirtAddr, Size) {
  // TODO: this
}

bool GlobalMap::Reserve(VirtAddr &, Size) {
  // TODO: this
}

void GlobalMap::ReserveAt(VirtAddr, Size) {
  // TODO: this
}

ansa::DepList GlobalMap::GetDependencies() {
  return ansa::DepList();
}

void GlobalMap::Initialize() {
  // create the temporary step allocator
  PhysAddr lastAddr = GetBootInfo()->GetKernelEnd();
  StepAllocator tempAllocator(lastAddr);
  pageAllocator = &tempAllocator;
  
  // TODO: here, setup the actual map
  
  // create the permanent buddy allocator
  void * ptr = (void *)tempAllocator.AllocAndMap(sizeof(BuddyAllocator));
  const RegionList & regions = GetBootInfo()->GetRegions();
  BuddyAllocator * buddy = new(ptr) BuddyAllocator(regions);
  buddy->Reserve(tempAllocator.GetLastAddress());
  
  pageAllocator = buddy;
}

}

}
