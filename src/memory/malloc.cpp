#include "malloc.hpp"
#include <anarch/api/panic>
#include <anarch/critical>
#include <anarch/align>

namespace anarch {

Malloc::Malloc(size_t pool, Allocator & _allocator)
  : poolSize(pool), allocator(_allocator) {
}

Malloc::~Malloc() {
  AssertNoncritical();
  while (firstSegment) {
    Segment * next = firstSegment->next;
    allocator.FreeAndUnmap((PhysAddr)next, (PhysSize)poolSize);
    firstSegment = next;
  }
}

bool Malloc::Alloc(void *& addr, size_t size) {
  if (size >= poolSize / 2) return false;
  
  if (!AllocNoNewSegment(addr, size)) {
    CreateSegment();
    return AllocNoNewSegment(addr, size);
  }
  return true;
}

void Malloc::Free(void * addr) {
  AssertNoncritical();
  firstLock.Seize();
  Segment * segment = firstSegment;
  firstLock.Release();
  
  while (segment) {
    ScopedLock scope(segment->lock);
    if (segment->allocator->OwnsPointer(addr)) {
      segment->allocator->Free(addr);
      return;
    }
  }
  
  Panic("Malloc::Free() - unowned memory region");
}

bool Malloc::Owns(void * ptr) {
  AssertNoncritical();
  firstLock.Seize();
  Segment * segment = firstSegment;
  firstLock.Release();
  
  while (segment) {
    ScopedLock scope(segment->lock);
    if (segment->allocator->OwnsPointer(ptr)) {
      return true;
    }
  }
  
  return false;
}

// PRIVATE //

bool Malloc::AllocNoNewSegment(void *& addr, size_t size) {
  AssertNoncritical();
  firstLock.Seize();
  Segment * segment = firstSegment;
  firstLock.Release();
  
  while (segment) {
    ScopedLock scope(segment->lock);
    
    addr = segment->allocator->Alloc(size);
    if (addr) return true;
    segment = segment->next;
  }
  
  return false;
}

void Malloc::CreateSegment() {
  VirtAddr freshMemory = allocator.AllocAndMap((PhysSize)poolSize);
  
  assert(!(freshMemory % ANARCH_OBJECT_ALIGN));
  
  Segment * segPtr = new((Segment *)freshMemory) Segment();
  // I *hate* when function names get this big
  segPtr->allocator = ANAlloc::Malloc::WrapRegion<ANAlloc::BBTree>
    ((uint8_t *)freshMemory, poolSize, 6, sizeof(Segment));

  ScopedLock scope(firstLock);
  segPtr->next = firstSegment;
  firstSegment = segPtr;
}

}
