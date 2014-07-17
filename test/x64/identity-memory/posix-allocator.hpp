#include <anarch/api/allocator>

namespace anarch {

namespace dummy {

class PosixAllocator : public Allocator {
public:
  virtual bool Alloc(PhysAddr & result, PhysSize size, PhysSize align);
  virtual void Free(PhysAddr address);
  virtual PhysSize Used();
  virtual PhysSize Available();
  virtual PhysSize Total();
};

}

}
