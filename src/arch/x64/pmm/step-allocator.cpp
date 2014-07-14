#include "step-allocator.hpp"
#include "region-list.hpp"
#include <anarch/api/panic>

namespace anarch {

namespace x64 {

StepAllocator::StepAllocator(PhysAddr start) : lastAddr(start) {
}

PhysAddr StepAllocator::AllocPage() {
  return AllocSize(0x1000);
}

void StepAllocator::FreePage(PhysAddr) {
  Panic("StepAllocator::FreePage() - nothing to do");
}

PhysAddr StepAllocator::AllocSize(PhysSize pageSize) {
  RegionList & regions = RegionList::GetGlobal();
  const ANAlloc::Region * reg = regions.FindRegion(lastAddr);
  if (!reg) {
    if (!(reg = regions.FindRegion(lastAddr - 1))) {
      Panic("StepAllocator::AllocSize() - lastAddr out of bounds");
    }
  }
  
  if (lastAddr % pageSize) lastAddr += pageSize - (lastAddr % pageSize);
  if (lastAddr > reg->GetEnd()) lastAddr = reg->GetEnd() - 1;
  
  while (lastAddr + pageSize > reg->GetEnd()) {
    reg = regions.NextRegion(reg);
    if (!reg) {
      Panic("StepAllocator::AllocSize() - out of memory");
    }
    lastAddr = reg->GetStart();
    if (lastAddr % pageSize) lastAddr += pageSize - (lastAddr % pageSize);
    if (lastAddr > reg->GetEnd()) lastAddr = reg->GetEnd() - 1;
  }
  
  PhysAddr res = lastAddr;
  lastAddr += pageSize;
  return res;
}

PhysAddr StepAllocator::GetLastAddress() {
  return lastAddr;
}

}

}
