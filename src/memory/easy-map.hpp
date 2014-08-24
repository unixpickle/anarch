#ifndef __ANARCH_EASY_MAP_HPP__
#define __ANARCH_EASY_MAP_HPP__

#include <anarch/api/memory-map>
#include <ansa/nocopy>

namespace anarch {

/**
 * This class makes it easy to map arbitrary misaligned regions of physical
 * memory to virtual memory. Additionally, it can provide a simple scoping
 * mechanism for memory mapping (if you're into that kinda thing).
 */
class EasyMap : public ansa::NoCopy {
private:
  VirtAddr mapStart;
  size_t pageSize;
  size_t pageAlign;
  size_t pageCount;
  
  VirtAddr start;
  
public:
  /**
   * @noncritical
   */
  EasyMap(PhysAddr start, size_t size);
  
  /**
   * @noncritical
   */
  ~EasyMap();
  
  /**
   * @ambicritical
   */
  VirtAddr GetStart() const;
};

}

#endif
