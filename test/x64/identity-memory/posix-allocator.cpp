#include "posix-allocator.hpp"
#include <cstddef>

extern "C" {
extern int posix_memalign(void ** memptr, size_t alignment, size_t size);
extern void free(void * ptr);
}

namespace anarch {

namespace dummy {

bool PosixAllocator::Alloc(PhysAddr & result, PhysSize size, PhysSize align) {
  void * ptr;
  if (posix_memalign(&ptr, (size_t)align, (size_t)size)) {
    return false;
  }
  result = (PhysAddr)ptr;
  ++allocationCount;
  return true;
}

void PosixAllocator::Free(PhysAddr address) {
  free((void *)address);
  --allocationCount;
}

PhysSize PosixAllocator::Used() {
  return 0x1234;
}

PhysSize PosixAllocator::Available() {
  return 0x100000000L;
}

PhysSize PosixAllocator::Total() {
  return 0x100000000L;
}

long PosixAllocator::GetAllocationCount() {
  return allocationCount;
}

}

}
