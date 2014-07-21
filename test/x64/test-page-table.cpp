#include "../../src/arch/x64/vmm/page-table.hpp"
#include "../../src/arch/x64/vmm/scratch.hpp"
#include "inverse-memory/inverse-scoped-scratch.hpp"
#include "inverse-memory/posix-allocator.hpp"
#include "inverse-memory/inverter.hpp"
#include <stdlib-api/scoped-pass>
#include <stdlib-api/assert>
#include <ansa/cstring>

using namespace anarch::x64;

void TestSetFirstPage();
void TestSetFragmented();
void TestUnset();

int main() {
  try {
    TestSetFirstPage();
    TestSetFragmented();
    TestUnset();
  } catch (const char * exc) {
    PrintError("received panic: ");
    Die(exc);
  }
  return 0;
}

void TestSetFirstPage() {
  ScopedPass pass("PageTable::[Set/Walk/FreeTable]() [first page]");
  
  anarch::dummy::PosixAllocator allocator;
  Scratch scratch(NULL); // inverse scratch
  
  PhysAddr thePML4;
  if (!allocator.Alloc(thePML4, 0x1000, 0x1000)) {
    Die("Failed to allocate PML4");
  }
  
  ansa::Bzero((void *)InvertPhys(thePML4), 0x1000);
  PageTable table(allocator, scratch, thePML4);
  
  Assert(allocator.GetAllocationCount() == 1);
  
  uint64_t entry;
  PhysSize entrySize = 0;
  Assert(table.Walk(0, entry, &entrySize) == -1);
  Assert(entrySize == 1UL << 39);
  Assert(allocator.GetAllocationCount() == 1);
  Assert(GetInverseScopedScratchUsage() == 0);
  
  table.Set(0, 0x1003, 3, 3);
  Assert(GetInverseScopedScratchUsage() == 0);
  Assert(allocator.GetAllocationCount() == 4);
  
  // ensure that the page is properly set and that Walk() returns the right
  // information
  Assert(table.Walk(0, entry, &entrySize) == 3);
  Assert(entry == 0x1003);
  Assert(entrySize == 0x1000);
  Assert(GetInverseScopedScratchUsage() == 0);

  Assert(table.Walk(100, entry, &entrySize) == 3);
  Assert(entry == 0x1003);
  Assert(entrySize == 0x1000);
  Assert(GetInverseScopedScratchUsage() == 0);

  Assert(table.Walk(0x1000, entry, &entrySize) == 3);
  Assert(entrySize == 0x1000);
  Assert(entry == 0);
  Assert(GetInverseScopedScratchUsage() == 0);
  
  Assert(table.Walk(0x200000, entry, &entrySize) == -1);
  Assert(entrySize == 0x200000);
  Assert(GetInverseScopedScratchUsage() == 0);
  
  table.FreeTable(0);
  Assert(allocator.GetAllocationCount() == 0);
}

void TestSetFragmented() {
  ScopedPass pass("PageTable::[Set/Walk/FreeTable]() [fragmented]");
  
  anarch::dummy::PosixAllocator allocator;
  Scratch scratch(NULL); // inverse scratch
  
  PhysAddr thePML4;
  if (!allocator.Alloc(thePML4, 0x1000, 0x1000)) {
    Die("Failed to allocate PML4");
  }
  ansa::Bzero((void *)InvertPhys(thePML4), 0x1000);
  PageTable table(allocator, scratch, thePML4);
  
  Assert(allocator.GetAllocationCount() == 1);
  Assert(GetInverseScopedScratchUsage() == 0);
  
  table.Set(0, 0x133800083, 3, 2);
  
  Assert(allocator.GetAllocationCount() == 3);
  Assert(GetInverseScopedScratchUsage() == 0);
  
  table.Set(0x400000, 0x1003, 3, 3);
  
  Assert(allocator.GetAllocationCount() == 4);
  Assert(GetInverseScopedScratchUsage() == 0);
  
  uint64_t entry;
  PhysSize entrySize;
  Assert(table.Walk(0, entry, &entrySize) == 2);
  Assert(entry == 0x133800083);
  Assert(entrySize == 0x200000);
  Assert(GetInverseScopedScratchUsage() == 0);
  
  entrySize = 0;
  Assert(table.Walk(0x200000, entry, &entrySize) == -1);
  Assert(entrySize == 0x200000);
  Assert(GetInverseScopedScratchUsage() == 0);
  
  Assert(table.Walk(0x400000, entry, &entrySize) == 3);
  Assert(entrySize == 0x1000);
  Assert(entry == 0x1003);
  Assert(GetInverseScopedScratchUsage() == 0);
  
  table.FreeTable(0);
  Assert(allocator.GetAllocationCount() == 0);
}

void TestUnset() {
  ScopedPass pass("PageTable::Unset()");
  
  anarch::dummy::PosixAllocator allocator;
  Scratch scratch(NULL); // inverse scratch
  
  PhysAddr thePML4;
  if (!allocator.Alloc(thePML4, 0x1000, 0x1000)) {
    Die("Failed to allocate PML4");
  }
  ansa::Bzero((void *)InvertPhys(thePML4), 0x1000);
  PageTable table(allocator, scratch, thePML4);
  
  Assert(allocator.GetAllocationCount() == 1);
  Assert(GetInverseScopedScratchUsage() == 0);
  
  table.Set(0, 0x133800083, 3, 2);
  
  Assert(allocator.GetAllocationCount() == 3);
  Assert(GetInverseScopedScratchUsage() == 0);
  
  table.Set(0x400000, 0x1003, 3, 3);
  
  Assert(allocator.GetAllocationCount() == 4);
  Assert(GetInverseScopedScratchUsage() == 0);
  
  Assert(table.Unset(0x400000));
  Assert(allocator.GetAllocationCount() == 3);
  
  Assert(table.Unset(0));
  Assert(allocator.GetAllocationCount() == 1);
  
  table.FreeTable(0);
  Assert(allocator.GetAllocationCount() == 0);
}
