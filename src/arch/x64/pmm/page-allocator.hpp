#ifndef __ANARCH_X64_PAGE_ALLOCATOR_HPP__
#define __ANARCH_X64_PAGE_ALLOCATOR_HPP__

#include <anarch/types>

namespace anarch {

namespace x64 {

class PageAllocator {
public:
  virtual PhysAddr AllocPage() = 0;
  virtual void FreePage(PhysAddr p) = 0;
};

}

}

#endif
