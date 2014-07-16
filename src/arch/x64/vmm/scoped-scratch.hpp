#include "scratch.hpp"

namespace anarch {

namespace x64 {

class ScopedScratch {
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