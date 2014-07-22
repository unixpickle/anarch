#include "easy-map.hpp"
#include <anarch/api/global-map>
#include <anarch/api/panic>
#include <anarch/critical>

namespace anarch {

EasyMap::EasyMap(PhysAddr _start, PhysSize _size) {
  AssertNoncritical();
  
  // TODO: here, attempt to use larger page sizes and then fall back
  
  pageAlign = GlobalMap::GetPageSizeAlign(0);
  pageSize = GlobalMap::GetPageSize(0);
  
  // this is a jumble of variables that some day will get cleaned up
  PhysAddr aligned = _start;
  PhysSize endSize = _size;
  PhysSize alignOffset = 0;
  if (aligned % pageAlign) {
    alignOffset = aligned % pageAlign;
    aligned -= alignOffset;
    endSize += alignOffset;
  }
  
  pageCount = endSize / pageSize + (endSize % pageSize ? 1 : 0);
  
  GlobalMap::Attributes attributes;
  GlobalMap::Size size(pageSize, pageCount);
  if (!GlobalMap::GetGlobal().Map(mapStart, aligned, size, attributes)) {
    Panic("EasyMap() - failed to map memory");
  }
  start = mapStart + alignOffset;
}

EasyMap::~EasyMap() {
  AssertNoncritical();
  GlobalMap::Size size(pageSize, pageCount);
  GlobalMap::GetGlobal().Unmap(mapStart, size);
}

VirtAddr EasyMap::GetStart() const {
  return start;
}

}
