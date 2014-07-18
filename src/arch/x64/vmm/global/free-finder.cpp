#include "free-finder.hpp"
#include "../scratch.hpp"

namespace anarch {

namespace x64 {

FreeFinder::FreeFinder(PageTable & pt) : pageTable(pt) {
}

bool FreeFinder::Alloc(VirtAddr & addr, PhysSize size, PhysSize align) {
  if (!CanAllocate(size, align)) {
    UpdateFreeRegion();
    if (!CanAllocate(size, align)) {
      return false;
    }
  }
  
  if (freeStart % align) {
    freeSize -= align - freeStart % align;
    freeStart += align - freeStart % align;
  }
  
  addr = freeStart;
  
  freeSize -= size;
  freeStart += size;
  
  return true;
}

void FreeFinder::Free(VirtAddr addr, PhysSize size) {
  if (addr == freeStart + freeSize) {
    // the chunk is right after the free region
    freeSize += size;
  } else if (addr + size == freeStart) {
    // the chunk is right before the free region
    freeSize += size;
    freeStart -= size;
  }
}

void FreeFinder::Reserve(VirtAddr addr, PhysSize size) {
  if (addr > freeStart + freeSize || addr + size <= freeStart) {
    return;
  }
  PhysSize leftSize = 0;
  PhysSize rightSize = 0;
  if (addr > freeStart) {
    leftSize = addr - freeStart;
  }
  if (addr + size < freeStart + freeSize) {
    rightSize = freeStart + freeSize - (addr + size);
  }
  if (!leftSize && !rightSize) {
    freeSize = 0;
    return;
  }
  if (leftSize > rightSize) {
    freeSize = leftSize;
  } else {
    freeStart += freeSize - rightSize;
    freeSize = rightSize;
  }
}

// PROTECTED //

void FreeFinder::UpdateFreeRegion() {
  VirtAddr addr = 0;
  while (addr < Scratch::StartAddr) {
    VirtAddr nextStart = addr;
    PhysSize nextSize = 0;
  
    while (1) {
      PhysSize pageSize;
      uint64_t entry = 0;
      int result = pageTable.Walk(nextStart + nextSize, entry, &pageSize);
      addr += pageSize;
      if (result >= 0 && entry) break;
      nextSize += pageSize;
      if (nextStart + nextSize >= Scratch::StartAddr) {
        nextSize = Scratch::StartAddr - nextStart;
        break;
      }
    }
  
    if (nextSize > freeSize) {
      freeSize = nextSize;
      freeStart = nextStart;
    }
  }
}

bool FreeFinder::CanAllocate(PhysSize size, PhysSize align) {
  if (!size) return true;
  
  PhysSize slice = 0;
  if (freeStart % align) {
    slice = align - (freeStart % align);
  }
  if (slice >= freeSize) return false;
  return freeSize - slice >= size;
}

}

}
