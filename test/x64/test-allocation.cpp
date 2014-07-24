#include "../../src/arch/x64/vmm/page-table.hpp"
#include "../../src/arch/x64/vmm/scratch.hpp"
#include "inverse-memory/inverse-global-map.hpp"
#include "inverse-memory/posix-allocator.hpp"
#include "inverse-memory/inverter.hpp"
#include <stdlib-api/scoped-pass>
#include <stdlib-api/assert>
#include <ansa/cstring>

using namespace anarch::x64;
using anarch::dummy::PosixAllocator;
using anarch::dummy::InverseGlobalMap;

void TestAllocAndMapSinglePage();
void TestAllocAndMapBigPages();
void TestAllocAndMapFallback();

int main() {
  TestAllocAndMapSinglePage();
  TestAllocAndMapBigPages();
  TestAllocAndMapFallback();
  return 0;
}

void TestAllocAndMapSinglePage() {
  ScopedPass pass("Allocator::AllocAndMapContiguous() [single page]");
  
  PosixAllocator allocator;
  InverseGlobalMap & map = InverseGlobalMap::GetGlobal();
  map.log.Clear();
  map.allowedPageSize = 0x1000;
  
  VirtAddr result;
  bool res = allocator.AllocAndMapContiguous(result, 0x1000, true);
  Assert(res);
  
  // check the allocator's log
  Assert(allocator.GetAllocationCount() == 1);
  Assert(allocator.log.GetCount() == 1);
  PosixAllocator::LogEntry entry = allocator.log[0];
  Assert(entry.allocation);
  Assert(entry.size == 0x1000);
  Assert(entry.align == 0x1000);
  
  // check the GlobalMap's log
  Assert(map.log.GetCount() == 1);
  InverseGlobalMap::LogEntry mapEntry;
  mapEntry = map.log[0];
  Assert(mapEntry.type == mapEntry.MapType);
  Assert(mapEntry.virtualAddr == result);
  Assert(mapEntry.physicalAddr == entry.address);
  Assert(mapEntry.pageSize == 0x1000);
  Assert(mapEntry.pageCount == 1);
  
  // unmap and free the page
  allocator.FreeAndUnmapContiguous(result, 0x1000);
  
  // check the allocator's log
  Assert(allocator.GetAllocationCount() == 0);
  Assert(allocator.log.GetCount() == 2);
  entry = allocator.log[1];
  Assert(!entry.allocation);
  Assert(entry.address == mapEntry.physicalAddr);
  
  // check the GlobalMap's log
  Assert(map.log.GetCount() == 3);
  mapEntry = map.log[1];
  Assert(mapEntry.type == mapEntry.ReadType);
  Assert(mapEntry.virtualAddr == result);
  Assert(mapEntry.physicalAddr == entry.address);
  mapEntry = map.log[2];
  Assert(mapEntry.type == mapEntry.UnmapType);
  Assert(mapEntry.virtualAddr == result);
  Assert(mapEntry.physicalAddr == entry.address);
  Assert(mapEntry.pageCount == 1);
}

void TestAllocAndMapBigPages() {
  ScopedPass pass("Allocator::AllocAndMapContiguous() [big pages]");
  
  PosixAllocator allocator;
  InverseGlobalMap & map = InverseGlobalMap::GetGlobal();
  map.log.Clear();
  map.allowedPageSize = 0x200000;
  
  VirtAddr result;
  bool res = allocator.AllocAndMapContiguous(result, 0x400000, true);
  Assert(res);
  
  // check the allocator's log
  Assert(allocator.GetAllocationCount() == 1);
  Assert(allocator.log.GetCount() == 1);
  PosixAllocator::LogEntry entry = allocator.log[0];
  Assert(entry.allocation);
  Assert(entry.size == 0x400000);
  Assert(entry.align == 0x200000);
  
  // check the GlobalMap's log
  Assert(map.log.GetCount() == 1);
  InverseGlobalMap::LogEntry mapEntry;
  mapEntry = map.log[0];
  Assert(mapEntry.type == mapEntry.MapType);
  Assert(mapEntry.virtualAddr == result);
  Assert(mapEntry.physicalAddr == entry.address);
  Assert(mapEntry.pageSize == 0x200000);
  Assert(mapEntry.pageCount == 2);
  
  // unmap and free the page
  allocator.FreeAndUnmapContiguous(result, 0x400000);
  
  // check the allocator's log
  Assert(allocator.GetAllocationCount() == 0);
  Assert(allocator.log.GetCount() == 2);
  entry = allocator.log[1];
  Assert(!entry.allocation);
  Assert(entry.address == mapEntry.physicalAddr);
  
  // check the GlobalMap's log
  Assert(map.log.GetCount() == 3);
  mapEntry = map.log[1];
  Assert(mapEntry.type == mapEntry.ReadType);
  Assert(mapEntry.virtualAddr == result);
  Assert(mapEntry.physicalAddr == entry.address);
  mapEntry = map.log[2];
  Assert(mapEntry.type == mapEntry.UnmapType);
  Assert(mapEntry.virtualAddr == result);
  Assert(mapEntry.physicalAddr == entry.address);
  Assert(mapEntry.pageCount == 2);
}

void TestAllocAndMapFallback() {
  ScopedPass pass("Allocator::AllocAndMapContiguous() [fallback]");
  
  PosixAllocator allocator;
  InverseGlobalMap & map = InverseGlobalMap::GetGlobal();
  map.log.Clear();
  map.allowedPageSize = 0x1000;
  
  // we will request 0x400000 but the GlobalMap will not allow a large page
  // mapping, so the Allocator will be forced to resort to smaller pages
  
  VirtAddr result;
  bool res = allocator.AllocAndMapContiguous(result, 0x400000, true);
  Assert(res);
  
  // check the allocator's log
  Assert(allocator.GetAllocationCount() == 1);
  Assert(allocator.log.GetCount() == 3);
  PosixAllocator::LogEntry entry = allocator.log[0];
  Assert(entry.allocation);
  Assert(entry.size == 0x400000);
  Assert(entry.align == 0x200000);
  entry = allocator.log[1];
  Assert(!entry.allocation);
  Assert(entry.address == allocator.log[0].address);
  entry = allocator.log[2];
  Assert(entry.allocation);
  Assert(entry.size == 0x400000);
  Assert(entry.align == 0x1000);
  
  // check the map's log
  Assert(map.log.GetCount() == 2);
  InverseGlobalMap::LogEntry mapEntry = map.log[0];
  Assert(mapEntry.type == mapEntry.MapType);
  Assert(mapEntry.physicalAddr == allocator.log[0].address);
  Assert(mapEntry.pageSize == 0x200000);
  Assert(mapEntry.pageCount == 2);
  mapEntry = map.log[1];
  Assert(mapEntry.type == mapEntry.MapType);
  Assert(mapEntry.physicalAddr == allocator.log[1].address);
  Assert(mapEntry.pageSize == 0x1000);
  Assert(mapEntry.pageCount == 0x400);
  
  // free the region
  allocator.FreeAndUnmapContiguous(result, 0x400000);
  
  // check the allocator's log
  Assert(allocator.GetAllocationCount() == 0);
  Assert(allocator.log.GetCount() == 4);
  entry = allocator.log[3];
  Assert(!entry.allocation);
  Assert(entry.address == allocator.log[2].address);
  
  // check the map's log
  Assert(map.log.GetCount() == 4);
  mapEntry = map.log[2];
  Assert(mapEntry.type == mapEntry.ReadType);
  Assert(mapEntry.virtualAddr == result);
  Assert(mapEntry.physicalAddr == entry.address);
  mapEntry = map.log[3];
  Assert(mapEntry.type == mapEntry.UnmapType);
  Assert(mapEntry.physicalAddr == entry.address);
  Assert(mapEntry.pageSize == 0x1000);
  Assert(mapEntry.pageCount == 0x400);
}
