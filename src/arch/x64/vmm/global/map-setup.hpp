#ifndef __ANARCH_X64_MAP_SETUP_HPP__
#define __ANARCH_X64_MAP_SETUP_HPP__

#include "free-finder.hpp"
#include "../scratch.hpp"
#include "../page-table.hpp"
#include "../../pmm/buddy-allocator.hpp"

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
  void GenerateFreeFinder();
  void GenerateBuddyAllocator();
  
  Scratch * GetScratch();
  PageTable * GetPageTable();
  FreeFinder * GetFreeFinder();
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
