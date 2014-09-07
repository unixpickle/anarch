#ifndef __ANARCH_X64_CPU_HPP__
#define __ANARCH_X64_CPU_HPP__

#include "../segments/tss.hpp"
#include "../timer/lapic-timer.hpp"
#include "../interrupts/apic/lapic.hpp"
#include <anarch/lock>
#include <anarch/stddef>
#include <anarch/api/thread>
#include <anarch/api/memory-map>
#include <ansa/macros>

namespace anarch {

namespace x64 {

class Cpu : public Thread {
public:
  static Cpu & GetCurrent();
  static void HandleWakeup();
  
  Cpu(); // @noncritical
  ~Cpu(); // @panic
  
  /**
   * Set a stack pointer in the TSS and the CPU data area so that it is used to
   * handle syscalls and interrupts from a user-space task.
   * @critical
   */
  void SetAsyncKernelTop(void * stack);
  
  void * GetStackTop(); // @ambicritical
  uint32_t GetApicId(); // @ambicritical
  LapicTimer & GetLapicTimer(); // @ambicritical
  Lapic & GetLapic(); // @ambicritical
  
  virtual anarch::Domain & GetDomain(); // @ambicritical
  virtual anarch::Timer & GetTimer(); // @critical
  virtual int GetPriority(); // @ambicritical
  virtual void RunAsync(void (*)()); // @critical
  virtual void RunAsync(void (*)(void *), void *); // @critical
  
protected:
  MemoryMap * currentMap = NULL;
  friend class Tlb;
  
private:
  static const size_t StackSize = 0x10000;
  
  struct LocalData {
    Cpu * thisCpu;
    void * syscallStack;
    void * userData = NULL;
  } ANSA_PACKED;
  
  anarch::CriticalLock wakeupLock;
  void (* wakeupFunction)(void *);
  void * wakeupArg;
  
  Lapic & lapic;
  LapicTimer lapicTimer;
  LocalData localData;
  Tss taskSegment;
  uint32_t apicId;
  void * stackTop;
};

}

}

#endif
