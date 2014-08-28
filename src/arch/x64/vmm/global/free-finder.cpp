#include "free-finder.hpp"

namespace anarch {

namespace x64 {

FreeFinder::FreeFinder(PageTable & pt) : pageTable(pt) {
}

bool FreeFinder::Alloc(VirtAddr & addr, size_t size, size_t align) {
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

void FreeFinder::Free(VirtAddr addr, size_t size) {
  if (addr == freeStart + freeSize) {
    // the chunk is right after the free region
    freeSize += size;
  } else if (addr + size == freeStart) {
    // the chunk is right before the free region
    freeSize += size;
    freeStart -= size;
  }
}

void FreeFinder::Reserve(VirtAddr addr, size_t size) {
  if (addr > freeStart + freeSize || addr + size <= freeStart) {
    return;
  }
  size_t leftSize = 0;
  size_t rightSize = 0;
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
  while (addr < KernelEnd) {
    VirtAddr nextStart = addr;
    size_t nextSize = 0;
  
    while (1) {
      size_t pageSize;
      uint64_t entry = 0;
      int result = pageTable.Walk(nextStart + nextSize, entry, &pageSize);
      addr += pageSize;
      if (result >= 0 && entry) break;
      nextSize += pageSize;
      if (nextStart + nextSize >= KernelEnd) {
        nextSize = KernelEnd - nextStart;
        break;
      }
    }
  
    if (nextSize > freeSize) {
      freeSize = nextSize;
      freeStart = nextStart;
    }
  }
}

bool FreeFinder::CanAllocate(size_t size, size_t align) {
  if (!size) return true;
  
  size_t slice = 0;
  if (freeStart % align) {
    slice = align - (freeStart % align);
  }
  if (slice >= freeSize) return false;
  return freeSize - slice >= size;
}

}

}
