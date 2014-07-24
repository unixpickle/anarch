#ifndef __ANARCH_API_MEMORY_MAP_HPP__
#define __ANARCH_API_MEMORY_MAP_HPP__

#include <anarch/types>
#include <ansa/nocopy>

namespace anarch {

class MemoryMap : public ansa::NoCopy {
public:
  struct Attributes {
    bool executable = true;
    bool writable = true;
    bool cachable = true;
  };
  
  struct Capabilities {
    /**
     * This is true when ReserveAt() is available.
     */
    bool placementReserve;
    
    /**
     * This is true when MapAt() and Reserve() are available.
     */
    bool placementMap;
    
    bool executableFlag;
    bool writableFlag;
    bool cachableFlag;
  };
  
  struct Size {
    PhysSize pageSize;
    PhysSize pageCount;
    
    Size(PhysSize s, PhysSize c) : pageSize(s), pageCount(c) {
    }
    
    inline PhysSize Bytes() const {
      return pageSize * pageCount;
    }
  };
  
  // @critical
  virtual void Set() = 0;
  
  // all @noncritical
  virtual bool Read(PhysAddr *, Attributes *, PhysSize *, VirtAddr) = 0;
  virtual bool Map(VirtAddr &, PhysAddr, Size, const Attributes &) = 0;
  virtual void MapAt(VirtAddr, PhysAddr, Size, const Attributes &) = 0;
  virtual void Unmap(VirtAddr, Size) = 0;
  virtual bool Reserve(VirtAddr &, Size) = 0;
  virtual void ReserveAt(VirtAddr, Size) = 0;
};

}

#endif
