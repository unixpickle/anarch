#ifndef __ANARCH_X64_STATE_HPP__
#define __ANARCH_X64_STATE_HPP__

#include "../interrupts/isr-types.hpp"
#include <anarch/api/state>
#include <anarch/stddef>

namespace anarch {

namespace x64 {

class State : public anarch::State {
public:
  static const size_t StackSize = 0x4000;
  
  State(uint64_t rip, uint64_t rdi, bool kernel);
  
  void * GetStackTop();
  
  // from anarch::State
  virtual ~State();
  virtual void Delete();
  virtual void Resume() ANSA_NORETURN;
  virtual void SuspendAndCall(void (*)());
  
private:
  uint64_t rdi;
  IsrStack iretInfo;
  void * stackStart;
};

}

}
  
#endif