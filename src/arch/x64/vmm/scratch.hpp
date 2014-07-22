#ifndef __ANARCH_X64_SCRATCH_HPP__
#define __ANARCH_X64_SCRATCH_HPP__

#include <anarch/types>
#include <anarch/lock>
#include <anarch/api/allocator>
#include <ansa/bit-list>
#include <ansa/nocopy>

namespace anarch {

namespace x64 {

class Scratch : public ansa::NoCopy {
public:
  static const int PTCount = 1;
  static const VirtAddr StartAddr = 0x7FC0000000L;
  
  Scratch(uint64_t * ptStart);
  virtual void CreateMappings(uint64_t * pdpt, Allocator & allocator);
  
  virtual bool Alloc(VirtAddr & addr, PhysAddr page); // @critical
  virtual void Reassign(VirtAddr addr, PhysAddr newAddr); // @critical
  virtual void Free(VirtAddr addr); // @critical
  
private:
  uint64_t * pageTables;
  ansa::BitList<PTCount * 0x200> bitmap;
  CriticalLock lock;
  
  static VirtAddr CellVirtual(size_t cellIdx);
};

static_assert(sizeof(Scratch) % 8 == 0, "invalid Scratch size");

}

}

#endif
