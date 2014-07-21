#include <anarch/api/allocator>
#include <ansa/atomic>

namespace anarch {

namespace dummy {

class PosixAllocator : public Allocator {
public:
  virtual bool Alloc(PhysAddr & result, PhysSize size, PhysSize align);
  virtual void Free(PhysAddr address);
  virtual PhysSize Used();
  virtual PhysSize Available();
  virtual PhysSize Total();
  
  long GetAllocationCount();
  
private:
  ansa::Atomic<long> allocationCount;
};

}

}
