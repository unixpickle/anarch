#include <anarch/api/memory-map>
#include <ansa/module>

namespace anarch {

namespace x64 {

class TLB : public ansa::Module {
public:
  static void Invlpg(VirtAddr addr);
  
  static void InitGlobal();
  static TLB & GetGlobal();
  
  /**
   * @critical
   */
  virtual void WillSetAddressSpace(MemoryMap & map);
  
  virtual void DistributeInvlpg(VirtAddr start, PhysSize size);
  
protected:
  ansa::DepList GetDependencies();
  virtual void Initialize();
};

}

}
