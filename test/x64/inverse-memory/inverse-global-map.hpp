#ifndef __ANARCH_TEST_INVERSE_GLOBAL_MAP_HPP__
#define __ANARCH_TEST_INVERSE_GLOBAL_MAP_HPP__

#include <anarch/api/global-map>

namespace anarch {

namespace dummy {

class InverseGlobalMap : public GlobalMap {
public:
  virtual void Set();

  virtual bool Read(PhysAddr * physOut, Attributes * attrOut,
                    PhysSize * sizeOut, VirtAddr addr);
  virtual bool Map(VirtAddr &, PhysAddr, Size, const Attributes &);
  virtual void MapAt(VirtAddr, PhysAddr, Size, const Attributes &);
  virtual void Unmap(VirtAddr, Size);
  virtual bool Reserve(VirtAddr &, Size);
  virtual void ReserveAt(VirtAddr, Size);
  
protected:
  ansa::DepList GetDependencies();
  virtual void Initialize();
};

}

}

#endif
