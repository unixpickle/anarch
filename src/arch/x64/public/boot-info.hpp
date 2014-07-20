#ifndef __ANARCH_X64_BOOT_INFO_HPP__
#define __ANARCH_X64_BOOT_INFO_HPP__

#include "region-list.hpp"
#include <anarch/types>

namespace anarch {

namespace x64 {

class BootInfo {
public:
  BootInfo(const RegionList & regs, PhysAddr end)
    : regions(regs), kernelEnd(end) {}
  
  const RegionList & GetRegions() const {
    return regions;
  }
  
  PhysAddr GetKernelEnd() const {
    return kernelEnd;
  }
  
private:
  const RegionList & regions;
  PhysAddr kernelEnd;
};

}

}

#endif
