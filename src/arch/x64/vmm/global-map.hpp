#ifndef __ANARCH_X64_GLOBAL_MAP_HPP__
#define __ANARCH_X64_GLOBAL_MAP_HPP__

#include <anarch/api/global-map>
#include <anarch/api/allocator>

namespace anarch {

namespace x64 {

class GlobalMap : public anarch::GlobalMap {
public:
  static void InitGlobal();
  static GlobalMap & GetGlobal();
  
  virtual Allocator & GetPageAllocator();
  
  // anarch::MemoryMap
  virtual void Set();
  virtual bool Map(VirtAddr &, PhysAddr, Size, const Attributes &);
  virtual void MapAt(VirtAddr, PhysAddr, Size, const Attributes &);
  virtual void Unmap(VirtAddr, Size);
  virtual bool Reserve(VirtAddr &, Size);
  virtual void ReserveAt(VirtAddr, Size);
  
protected:
  // anarch::Module
  virtual ansa::DepList GetDependencies();
  virtual void Initialize();
  
private:
  Allocator * pageAllocator;
};

}

}

#endif
