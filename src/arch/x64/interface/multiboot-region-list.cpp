#include "multiboot-region-list.hpp"
#include "multiboot.hpp"
#include <anarch/panic>

namespace anarch {

namespace x64 {

MultibootRegionList::MultibootRegionList(void * mbootPtr) {
  MultibootBootInfo * multibootPtr = (MultibootBootInfo *)mbootPtr;
  
  uint32_t mmapLen = multibootPtr->mmap_length;
  uint32_t mmapAddr = multibootPtr->mmap_addr;
  if (mmapAddr + mmapLen > 0x100000) {
    Panic("GRUB memory map is placed beyond 1MB -- might be destroyed!");
  }
  
  // generate ANAlloc::Region objects in a loop
  uintptr_t longAddr = (uintptr_t)mmapAddr;
  MultibootMmapInfo * info = NULL;
  MultibootMmapInfo * next = (MultibootMmapInfo *)longAddr;
  while (mmapLen > 0) {
    info = next;
    next = (MultibootMmapInfo *)((uintptr_t)info + info->size + 4);
    mmapLen -= info->size + 4;
    
    if (info->type != 1) continue;
    
    // simplify the process by assuming that the first MB is contiguous (even)
    // though really it's not at all.
    if (info->base_addr + info->length <= 0x100000) continue;
    
    // get the range
    ANAlloc::UInt start = (ANAlloc::UInt)info->base_addr;
    ANAlloc::UInt len = (ANAlloc::UInt)info->length;
    
    // if this region is in the lower MB, we will stretch it out to the
    // beginning of the address space.
    if (start <= 0x100000) {
      len += start;
      start = 0;
    }
    
    ANAlloc::Region region(start, len);
    if (start < 0x100000000L && start + len > 0x100000000L) {
      // this will probably never happen, but if it does we need to split the
      // regions for ANAlloc to be able to differentiate between upper and
      // lower memory
      ANAlloc::UInt lowSize = (ANAlloc::UInt)(0x100000000L - start);
      ANAlloc::Region lowerRegion(start, lowSize);
      ANAlloc::Region upperRegion(0x100000000L, len - lowSize);
      AddRegion(lowerRegion);
      AddRegion(upperRegion);
    } else {
      AddRegion(region);
    }
  }
  if (!lowerRegions.GetCount()) {
    Panic("MultibootRegionList::Initialize() - no lower regions found");
  }
}

void MultibootRegionList::AddRegion(const ANAlloc::Region & region) {
  ANAlloc::FixedRegionList<MaximumCount> * list = NULL;
  
  if (region.GetStart() < 0x100000000L) {
    list = &lowerRegions;
  } else {
    list = &upperRegions;
  }
  
  int insertIndex = list->GetCount();
  for (int i = 0; i < list->GetCount(); i++) {
    if ((*list)[i].GetStart() > region.GetStart()) {
      insertIndex = i;
      break;
    }
  }
  if (!list->Insert(region, insertIndex)) {
    Panic("MultibootRegionList::AddRegion() - region overflow");
  }
}

}

}