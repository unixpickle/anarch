#ifndef __ANARCH_X64_REGION_LIST_HPP__
#define __ANARCH_X64_REGION_LIST_HPP__

#include <analloc2>

namespace anarch {

namespace x64 {

class RegionList {
public:
  virtual const ANAlloc::RegionList & GetLowerRegions() const = 0;
  virtual const ANAlloc::RegionList & GetUpperRegions() const = 0;
  
  virtual const ANAlloc::Region * FindRegion(PhysAddr addr) const;
  virtual const ANAlloc::Region * NextRegion(const ANAlloc::Region * reg)
    const;
};

}

}

#endif
