#include <anarch/types>
#include <ansa/module>

namespace anarch {

namespace x64 {

class TLB : public ansa::Module {
public:
  static void Invlpg(VirtAddr addr);
  
  static void InitGlobal();
  static TLB & GetGlobal();
  
  virtual void WillSetAddressSpace(MemoryMap & map);
  
protected:
  ansa::DepList GetDependencies();
  virtual void Initialize();
};

}

}
