#ifndef __ANARCH_X64_CPU_HPP__
#define __ANARCH_X64_CPU_HPP__

#include <anarch/stddef>
#include <anarch/api/thread>
#include <ansa/macros>

namespace anarch {

namespace x64 {

class Cpu : public Thread {
public:
  static Cpu & GetCurrent();
  
  Cpu(); // @noncritical
  ~Cpu(); // @panic
  
  void * GetStackTop();
  
  virtual anarch::Domain & GetDomain(); // @ambicritical
  virtual anarch::Timer & GetTimer(); // @critical
  virtual int GetPriority();
  
private:
  static const size_t StackSize = 0x10000;
  
  struct LocalData {
    Cpu * thisCpu;
    void * userData;
    void * stackTop;
  } ANSA_PACKED;
  
  LocalData localData;
};

}

}

#endif
