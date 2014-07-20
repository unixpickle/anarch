#include <anarch/malloc>
#include <ansa/module>

namespace anarch {

namespace x64 {

class GlobalMalloc : public ansa::Module {
public:
  static void InitGlobal();
  static GlobalMalloc & GetGlobal();
  
  VirtualAllocator & GetVirtualAllocator();
  
protected:
  virtual ansa::DepList GetDependencies();
  virtual void Initialize();
  
private:
  char mallocBuf[sizeof(Malloc)] ANSA_ALIGNED(8);
};

}

}
