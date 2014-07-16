#ifndef __ANARCH_X64_MAP_SETUP_HPP__
#define __ANARCH_X64_MAP_SETUP_HPP__

#include "../pmm/buddy-allocator.hpp"
#include "scratch.hpp"
#include "page-table.hpp"

namespace anarch {

namespace x64 {

/**
 * Configure the GlobalMap nice and easy
 */
class MapSetup {
public:
  MapSetup();
  
  void GenerateMap();
  void GenerateScratch();
  void GeneratePageTable();
  void GenerateBuddyAllocator();
  
  Scratch * GetScratch();
  PageTable * GetPageTable();
  BuddyAllocator * GetBuddyAllocator();
  
  PhysAddr GetPDPT();
  
private:
  PhysAddr kernelEnd;
  PhysAddr scratchEnd;
  PhysAddr reservedEnd;
  StepAllocator stepAllocator;
  
  PhysAddr pdpt, pml4;
  int pdtOffset = 0;
  int pdptOffset = 0;
  PhysAddr currentPDT = 0;
  VirtAddr firstUnmapped = 0;
   
  void MapNext(PhysSize & linearSize);
};

}

}

#endif
