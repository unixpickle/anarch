#include "cpu.hpp"
#include "../segments/local-segment.hpp"
#include "../domains/domain-list.hpp"
#include <anarch/api/panic>
#include <anarch/critical>

namespace anarch {

Thread & Thread::GetCurrent() {
  return x64::Cpu::GetCurrent();
}

void * Thread::GetUserInfo() {
  void * res;
  __asm__("mov %%gs:(0x8), %0" : "=r" (res) : : "memory");
  return res;
}

void Thread::SetUserInfo(void * info) {
  __asm__("mov %0, %%gs:(0x8)" : : "r" (info) : "memory");
}

namespace x64 {

Cpu & Cpu::GetCurrent() {
  Cpu * ptr;
  __asm__("mov %%gs:(0x0), %0" : "=r" (ptr));
  return *ptr;
}

Cpu::Cpu() {
  localData.thisCpu = this;
  localData.userData = NULL;
  
  void * stack;
  if (!GetDomain().GetVirtualAllocator().Alloc(stack, StackSize)) {
    Panic("Cpu::Cpu() - failed to allocate stack");
  }
  localData.stackTop = (void *)((uint64_t)stack + StackSize);
  
  ScopedCritical critical;
  LocalSegment::Write((uint64_t)&localData);
}

Cpu::~Cpu() {
  Panic("Cpu::~Cpu() - CPU cannot be destroyed!");
}

anarch::Domain & Cpu::GetDomain() {
  return DomainList::GetGlobal()[0];
}

anarch::Timer & Cpu::GetTimer() {
  // TODO: this
  Panic("TODO: implement Cpu::GetTimer()");
  return *(anarch::Timer *)0;
}

int Cpu::GetPriority() {
  // TODO: return the index in the CPU core
  return 0;
}

void * Cpu::GetStackTop() {
  return localData.stackTop;
}

}

}
