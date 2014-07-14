#ifndef __ANARCH_X64_REGION_LIST_HPP__
#define __ANARCH_X64_REGION_LIST_HPP__

#include <ansa/module>
#include <anarch/types>
#include <analloc2>

namespace anarch {

namespace x64 {

class RegionList : public ansa::Module {
public:
  static const int MaximumCount = 8;  
  
  static void InitGlobal(void * mbootPtr);
  static RegionList & GetGlobal();
  
  const ANAlloc::RegionList & GetLowerRegions();
  const ANAlloc::RegionList & GetUpperRegions();
  
  const ANAlloc::Region * FindRegion(PhysAddr addr);
  const ANAlloc::Region * NextRegion(const ANAlloc::Region * reg);
  
protected:
  ANAlloc::FixedRegionList<MaximumCount> lowerRegions;
  ANAlloc::FixedRegionList<MaximumCount> upperRegions;
  
  virtual void Initialize();
  virtual ansa::DepList GetDependencies();
  
  void AddRegion(const ANAlloc::Region & region);
};

}

}

#endif
