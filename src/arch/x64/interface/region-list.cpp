#include "region-list.hpp"

namespace anarch {

namespace x64 {

const ANAlloc::Region * RegionList::FindRegion(PhysAddr addr) const {
  for (int i = 0; i < GetLowerRegions().GetCount(); i++) {
    if (GetLowerRegions()[i].Contains((ANAlloc::UInt)addr)) {
      return &GetLowerRegions()[i];
    }
  }
  for (int i = 0; i < GetUpperRegions().GetCount(); i++) {
    if (GetUpperRegions()[i].Contains((ANAlloc::UInt)addr)) {
      return &GetUpperRegions()[i];
    }
  }
  return NULL;
}

const ANAlloc::Region * RegionList::NextRegion(const ANAlloc::Region * reg)
  const {
  for (int i = 0; i < GetLowerRegions().GetCount(); i++) {
    if (GetLowerRegions()[i].GetStart() > reg->GetStart()) {
      return &GetLowerRegions()[i];
    }
  }
  for (int i = 0; i < GetUpperRegions().GetCount(); i++) {
    if (GetUpperRegions()[i].GetStart() > reg->GetStart()) {
      return &GetUpperRegions()[i];
    }
  }
  return NULL;
}

}

}
