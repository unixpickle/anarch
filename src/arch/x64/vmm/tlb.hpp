#ifndef __ANARCH_X64_Tlb_HPP__
#define __ANARCH_X64_Tlb_HPP__

#include <anarch/api/memory-map>
#include <anarch/lock>
#include <ansa/module>

namespace anarch {

namespace x64 {

class Tlb : public ansa::Module {
public:
  static const VirtAddr UserStart = 0x8000000000UL;
  
  static void Invlpg(VirtAddr addr); // @critical
  static void Invlpgs(VirtAddr, PhysSize); // @critical
  
  static void InitGlobal();
  static Tlb & GetGlobal();
  
  /**
   * @critical
   */
  virtual void WillSetAddressSpace(MemoryMap & map);
  
  /**
   * @noncritical
   */
  virtual void DistributeInvlpg(VirtAddr start, PhysSize size);
  
protected:
  ansa::DepList GetDependencies();
  virtual void Initialize();
  
private:
  NoncriticalLock lock;
  static void HandleNotification();
  void DistributeKernel(VirtAddr, PhysSize); // @critical
  void DistributeUser(VirtAddr, PhysSize); // @critical
};

}

}

#endif
