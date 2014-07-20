#ifndef __ANARCH_X64_MULTIBOOT_REGION_LIST_HPP__
#define __ANARCH_X64_MULTIBOOT_REGION_LIST_HPP__

#include "region-list.hpp"

namespace anarch {

namespace x64 {

class MultibootRegionList : public RegionList {
public:
  MultibootRegionList(void * mbootPtr);
  
  virtual const ANAlloc::RegionList & GetLowerRegions() const;
  virtual const ANAlloc::RegionList & GetUpperRegions() const;
  
private:
  static const int MaximumCount = 8;  
  
  ANAlloc::FixedRegionList<MaximumCount> lowerRegions;
  ANAlloc::FixedRegionList<MaximumCount> upperRegions;
  
  void AddRegion(const ANAlloc::Region & region);
};

}

}

#endif
