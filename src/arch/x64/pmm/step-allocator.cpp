#include "step-allocator.hpp"
#include <anarch/x64/init>
#include <anarch/api/panic>

namespace anarch {

namespace x64 {

StepAllocator::StepAllocator(PhysAddr start) : lastAddr(start) {
}

PhysAddr StepAllocator::GetLastAddress() {
  return lastAddr;
}

bool StepAllocator::Alloc(PhysAddr & addr, PhysSize size, PhysSize align) {
  const RegionList & regions = GetBootInfo()->GetRegions();
  
  const ANAlloc::Region * reg = regions.FindRegion(lastAddr);
  if (!reg) {
    if (!(reg = regions.FindRegion(lastAddr - 1))) {
      return false;
    }
  }

  if (lastAddr % align) {
    lastAddr += align - (lastAddr % align);
  }
  
  if (lastAddr > (PhysAddr)reg->GetEnd()) {
    lastAddr = (PhysAddr)reg->GetEnd() - 1;
  }

  while (lastAddr + size > (PhysAddr)reg->GetEnd()) {
    reg = regions.NextRegion(reg);
    if (!reg) return false;
    
    lastAddr = (PhysAddr)reg->GetStart();
    if (lastAddr % align) {
      lastAddr += align - (lastAddr % align);
    }
    if (lastAddr > (PhysAddr)reg->GetEnd()) {
      lastAddr = (PhysAddr)reg->GetEnd() - 1;
    }
  }

  addr = lastAddr;
  lastAddr += size;
  return true;
}

void StepAllocator::Free(PhysAddr) {
  Panic("StepAllocator::Free() - unsupported");
}

bool StepAllocator::Owns(PhysAddr) {
  Panic("StepAllocator::Owns() - unsupported");
}

PhysSize StepAllocator::Used() {
  Panic("StepAllocator::Used() - unsupported");
}

PhysSize StepAllocator::Available() {
  Panic("StepAllocator::Available() - unsupported");
}

PhysSize StepAllocator::Total() {
  Panic("StepAllocator::Total() - unsupported");
}

}

}
