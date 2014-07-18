#include "map-setup.hpp"
#include <anarch/x64/init>
#include <anarch/api/panic>
#include <ansa/cstring>
#include <ansa/math>

namespace anarch {

namespace x64 {

MapSetup::MapSetup()
  : kernelEnd(ansa::Align<PhysSize>(GetBootInfo()->GetKernelEnd(), 0x1000)),
    scratchEnd(kernelEnd + 0x1000 * Scratch::PTCount),
    reservedEnd(scratchEnd + ansa::Align<PhysSize>(sizeof(Scratch)
      + sizeof(BuddyAllocator) + sizeof(PageTable)
      + sizeof(FreeFinder), 0x1000)),
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
  
  pdtOffset = 0x200;
  pdptOffset = -1;
  
  PhysSize linearSize = (PhysSize)reservedEnd;
  while (linearSize) {
    MapNext(linearSize);
  }
}

void MapSetup::GenerateScratch() {
  uint64_t * scratchStart = (uint64_t *)kernelEnd;
  new(GetScratch()) Scratch(scratchStart);
  GetScratch()->CreateMappings((uint64_t *)pdpt, stepAllocator);
}

void MapSetup::GeneratePageTable() {
  new(GetPageTable()) PageTable(stepAllocator, *GetScratch(), pml4);
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

Scratch * MapSetup::GetScratch() {
  return (Scratch *)scratchEnd;
}

PageTable * MapSetup::GetPageTable() {
  static_assert(sizeof(Scratch) % 8 == 0, "bad PageTable alignment");
  return (PageTable *)(scratchEnd + sizeof(Scratch));
}

FreeFinder * MapSetup::GetFreeFinder() {
  static_assert(sizeof(PageTable) % 8 == 0, "bad FreeFinder alignment");
  return (FreeFinder *)(scratchEnd + sizeof(Scratch) + sizeof(PageTable));
}

BuddyAllocator * MapSetup::GetBuddyAllocator() {
  static_assert(sizeof(FreeFinder) % 8 == 0, "bad BuddyAllocator alignment");
  return (BuddyAllocator *)(scratchEnd + sizeof(Scratch) + sizeof(PageTable)
    + sizeof(FreeFinder));
}

PhysAddr MapSetup::GetPDPT() {
  return (PhysAddr)pdpt;
}

// PRIVATE //

void MapSetup::MapNext(PhysSize & linearSize) {
  if (pdtOffset == 0x200) {
    pdtOffset = 0;
    if (!stepAllocator.Alloc(currentPDT, 0x1000, 0x1000)) {
      Panic("MapSetup::MapNext() - failed to allocate PDT");
    }
    ansa::Bzero((void *)currentPDT, 0x1000);
    ((uint64_t *)pdpt)[++pdptOffset] = currentPDT | 3;
  }
  ((uint64_t *)currentPDT)[pdtOffset++] = firstUnmapped | 0x183;
  if (linearSize <= 0x200000) linearSize = 0;
  else linearSize -= 0x200000;
}

}

}
