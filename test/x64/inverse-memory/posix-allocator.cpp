#include "posix-allocator.hpp"
#include "inverter.hpp"
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
    LogEntry * entry = new LogEntry(true, 0, 0, 0);
    log.Push(entry);
    return false;
  }
  result = InvertPhys((PhysAddr)ptr);
  ++allocationCount;
  
  LogEntry * entry = new LogEntry(true, result, size, align);
  log.Push(entry);
  
  return true;
}

void PosixAllocator::Free(PhysAddr address) {
  free((void *)InvertPhys(address));
  
  LogEntry * entry = new LogEntry(false, address, 0, 0);
  log.Push(entry);
  
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
