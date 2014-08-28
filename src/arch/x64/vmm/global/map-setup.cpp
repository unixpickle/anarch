#include "map-setup.hpp"
#include <anarch/x64/init>
#include <anarch/api/panic>
#include <ansa/cstring>
#include <ansa/math>

namespace anarch {

namespace x64 {

MapSetup::MapSetup()
  : kernelEnd(ansa::Align<size_t>(GetBootInfo()->GetKernelEnd(), 0x1000)),
    reservedEnd(kernelEnd + ansa::Align<size_t>(sizeof(BuddyAllocator)
      + sizeof(PageTable) + sizeof(FreeFinder), 0x1000)),
    stepAllocator(reservedEnd) {
}

void MapSetup::GenerateMap() {
  if (!stepAllocator.Alloc(pml4, 0x1000, 0x1000)) {
    Panic("MapSetup::Run() - failed to allocate PML4");
  }
  if (!stepAllocator.Alloc(pdpt, 0x1000, 0x1000)) {
    Panic("MapSetup::Run() - failed to allocate PDPT");
  }
  
  ansa::Bzero((void *)pml4, 0x1000);
  ansa::Bzero((void *)pdpt, 0x1000);
  
  uint64_t * pml4Buf = (uint64_t *)pml4;
  pml4Buf[0] = pdpt | 3;
  pml4Buf[0x1ff] = pml4 | 3; // fractal mapping
  
  pdtOffset = 0x200;
  pdptOffset = -1;
  
  size_t linearSize = (size_t)reservedEnd;
  while (linearSize) {
    MapNext(linearSize);
  }
}

void MapSetup::GeneratePageTable() {
  new(GetPageTable()) PageTable(stepAllocator, pml4);
}

void MapSetup::GenerateFreeFinder() {
  new(GetFreeFinder()) FreeFinder(*GetPageTable());
}

void MapSetup::GenerateBuddyAllocator() {
  const RegionList & regions = GetBootInfo()->GetRegions();
  new(GetBuddyAllocator()) BuddyAllocator(regions, stepAllocator);
  GetBuddyAllocator()->Reserve(stepAllocator.GetLastAddress());
  GetPageTable()->SetAllocator(*GetBuddyAllocator());
}

PageTable * MapSetup::GetPageTable() {
  return (PageTable *)kernelEnd;
}

FreeFinder * MapSetup::GetFreeFinder() {
  static_assert(sizeof(PageTable) % 8 == 0, "bad FreeFinder alignment");
  return (FreeFinder *)(kernelEnd + sizeof(PageTable));
}

BuddyAllocator * MapSetup::GetBuddyAllocator() {
  static_assert(sizeof(FreeFinder) % 8 == 0, "bad BuddyAllocator alignment");
  return (BuddyAllocator *)(kernelEnd + sizeof(PageTable)
    + sizeof(FreeFinder));
}

PhysAddr MapSetup::GetPdpt() {
  return (PhysAddr)pdpt;
}

// PRIVATE //

void MapSetup::MapNext(size_t & linearSize) {
  if (pdtOffset == 0x200) {
    pdtOffset = 0;
    if (!stepAllocator.Alloc(currentPDT, 0x1000, 0x1000)) {
      Panic("MapSetup::MapNext() - failed to allocate PDT");
    }
    ansa::Bzero((void *)currentPDT, 0x1000);
    ((uint64_t *)pdpt)[++pdptOffset] = currentPDT | 3;
  }
  ((uint64_t *)currentPDT)[pdtOffset++] = firstUnmapped | 0x183;
  firstUnmapped += 0x200000; // forgetting this line ruined my evening
  if (linearSize <= 0x200000) linearSize = 0;
  else linearSize -= 0x200000;
}

}

}
