#ifndef __ANARCH_X64_GLOBAL_MAP_HPP__
#define __ANARCH_X64_GLOBAL_MAP_HPP__

#include "../page-table.hpp"
#include "free-finder.hpp"
#include <anarch/api/global-map>
#include <anarch/api/allocator>
#include <anarch/lock>

namespace anarch {

namespace x64 {

class GlobalMap : public anarch::GlobalMap {
public:
  static void InitGlobal();
  static GlobalMap & GetGlobal();
  
  virtual PageTable & GetPageTable();
  virtual FreeFinder & GetFreeFinder();
  virtual Allocator & GetPageAllocator();
  virtual PhysAddr GetPdpt();
  
  // anarch::MemoryMap
  virtual void Set();
  virtual bool Read(PhysAddr *, Attributes *, size_t *, VirtAddr);
  virtual bool Map(VirtAddr &, PhysAddr, Size, const Attributes &);
  virtual void MapAt(VirtAddr, PhysAddr, Size, const Attributes &);
  virtual void Unmap(VirtAddr, Size);
  virtual void UnmapAndReserve(VirtAddr, Size);
  virtual bool Reserve(VirtAddr &, Size);
  virtual void ReserveAt(VirtAddr, Size);
  virtual void Unreserve(VirtAddr, Size);
  virtual void Rereserve(VirtAddr, Size, size_t);
  
protected:
  // anarch::Module
  virtual ansa::DepList GetDependencies();
  virtual void Initialize();
  
private:
  PageTable * pageTable;
  FreeFinder * freeFinder;
  Allocator * pageAllocator;
  PhysAddr pdpt;
  
  NoncriticalLock lock;
};

}

}

#endif
