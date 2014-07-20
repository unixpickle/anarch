#ifndef __ANARCH_MALLOC_HPP__
#define __ANARCH_MALLOC_HPP__

#include <anarch/api/allocator>
#include <anarch/api/virtual-allocator>
#include <anarch/lock>
#include <analloc2>

namespace anarch {

/**
 * This class provides VirtualAllocator functionality using analloc2 and
 * wrapping a physical allocator. An architecture may choose to use this, or an
 * operating system kernel itself.
 */
class Malloc : public VirtualAllocator {
public:
  Malloc(size_t poolSize, Allocator & allocator); // @ambicritical
  virtual ~Malloc(); // @noncritical
  
  virtual bool Alloc(void *& addr, size_t size); // @noncritical
  virtual void Free(void * addr); // @noncritical
  
protected:
  size_t poolSize;
  
  struct Segment {
    ANAlloc::Malloc * allocator;
    Segment * next;
    NoncriticalLock lock;
  };
  
  Allocator & allocator;
  Segment * firstSegment = NULL;
  NoncriticalLock firstLock;
  
  bool AllocNoNewSegment(void *& addr, size_t size);
  void CreateSegment();
};

}

#endif
