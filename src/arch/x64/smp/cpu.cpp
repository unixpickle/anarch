#include "cpu.hpp"
#include "../segments/local-segment.hpp"
#include "../segments/gdt.hpp"
#include "../domains/domain-list.hpp"
#include "../interrupts/apic/lapic-module.hpp"
#include "../interrupts/vectors.hpp"
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

void Cpu::HandleWakeup() {
  LapicModule::GetGlobal().GetLapic().SendEoi();
  Cpu & cur = GetCurrent();
  
  // wakeupFunction has an atomic value because of automatic alignment
  cur.wakeupFunction();
}

Cpu::Cpu() {
  localData.thisCpu = this;
  localData.userData = NULL;
  
  void * stack;
  if (!GetDomain().Alloc(stack, StackSize)) {
    Panic("Cpu::Cpu() - failed to allocate stack");
  }
  stackTop = (void *)((uint64_t)stack + StackSize);
  
  ScopedCritical critical;
  LocalSegment::Write((uint64_t)&localData);
  
  for (int i = 0; i < 7; ++i) {
    taskSegment.ist[i] = (uint64_t)stackTop;
  }
  
  TssDescriptor desc(&taskSegment);
  uint16_t seg = Gdt::GetGlobal().PushTssDescriptor(desc);
  __asm__ __volatile__("ltr %%ax" : : "a" (seg));
  
  apicId = LapicModule::GetGlobal().GetLapic().GetId();
}

Cpu::~Cpu() {
  Panic("Cpu::~Cpu() - CPU cannot be destroyed!");
}

void Cpu::SetAsyncKernelTop(void * stack) {
  taskSegment.rsp[0] = (uint64_t)stack;
  localData.syscallStack = stack;
}

void * Cpu::GetStackTop() {
  return stackTop;
}

uint32_t Cpu::GetApicId() {
  return apicId;
}

LapicTimer & Cpu::GetLapicTimer() {
  return lapicTimer;
}

anarch::Domain & Cpu::GetDomain() {
  return DomainList::GetGlobal()[0];
}

anarch::Timer & Cpu::GetTimer() {
  return GetLapicTimer();
}

int Cpu::GetPriority() {
  // TODO: return the index in the CPU core
  return 0;
}

void Cpu::RunAsync(void (* func)()) {
  AssertCritical();
  wakeupFunction = func;
  Lapic & lapic = LapicModule::GetGlobal().GetLapic();
  lapic.SendIpi(GetApicId(), IntVectors::Wakeup);
}

}

}
