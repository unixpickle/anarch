#ifndef __ANARCH_X64_Tlb_HPP__
#define __ANARCH_X64_Tlb_HPP__

#include <anarch/api/memory-map>
#include <anarch/lock>
#include <ansa/module>

namespace anarch {

namespace x64 {

class Tlb : public ansa::Module {
public:
  static const VirtAddr KernelEnd = 0x8000000000UL;
  
  static void Invlpg(VirtAddr addr); // @critical
  static void Invlpgs(VirtAddr, size_t); // @critical
  
  static void InitGlobal();
  static Tlb & GetGlobal();
  
  /**
   * Call this with `NULL` to indicate the global map.
   * @critical
   */
  virtual void WillSetAddressSpace(MemoryMap * map);
  
  /**
   * Distribute a TLB flush for the current map. If the memory is in the
   * kernel's address space (< 0x8000000000), it will be distributed to every
   * CPU.
   * @noncritical
   */
  virtual void DistributeInvlpg(VirtAddr start, size_t size);
  
protected:
  ansa::DepList GetDependencies();
  virtual void Initialize();
  
private:
  NoncriticalLock lock;
  static void HandleNotification();
  void DistributeKernel(VirtAddr, size_t); // @critical
  void DistributeUser(VirtAddr, size_t); // @critical
};

}

}

#endif
