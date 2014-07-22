#ifndef __ANARCH_X64_SCOPED_SCRATCH_X64_HPP__
#define __ANARCH_X64_SCOPED_SCRATCH_X64_HPP__

#include "scratch.hpp"
#include <ansa/nocopy>

namespace anarch {

namespace x64 {

class ScopedScratch : public ansa::NoCopy {
public:
  ScopedScratch(Scratch &, PhysAddr); // @critical
  ~ScopedScratch(); // @critical
  
  void * GetPointer(); // @ambicritical
  VirtAddr GetVirtAddr(); // @ambicritical
  
  void Reassign(PhysAddr); // @critical
  void InvalidateCache(); // @critical
  
protected:
  Scratch & scratch;
  VirtAddr addr;
};

template <typename T>
class TypedScratch : public ScopedScratch {
public:
  /**
   * @critical
   */
  TypedScratch(Scratch & s, PhysAddr p) : ScopedScratch(s, p) {
  }
  
  /**
   * @ambicritical
   */
  inline T * GetTypedPointer() {
    return (T *)addr;
  }
  
  /**
   * @critical
   */
  inline T & operator[](int idx) {
    return GetTypedPointer()[idx];
  }
};

}

}

#endif
