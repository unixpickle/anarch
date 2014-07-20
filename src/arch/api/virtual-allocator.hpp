#ifndef __ANARCH_VIRTUAL_ALLOCATOR_HPP__
#define __ANARCH_VIRTUAL_ALLOCATOR_HPP__

#include <anarch/types>
#include <anarch/stddef>

namespace anarch {

class VirtualAllocator {
public:
  virtual bool Alloc(void *& addr, size_t size) = 0; // @noncritical
  virtual void Free(void * addr) = 0; // @noncritical
};

}

#endif
