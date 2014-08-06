#include "state.hpp"
#include "../smp/cpu.hpp"
#include <anarch/api/domain>
#include <anarch/api/panic>
#include <anarch/critical>

namespace anarch {

State & State::NewKernel(void (* fn)()) {
  return *Domain::GetCurrent().New<x64::State>((uint64_t)fn, 0, true);
}

State & State::NewKernel(void (* fn)(void *), void * arg) {
  return *Domain::GetCurrent().New<x64::State>((uint64_t)fn, (uint64_t)arg,
                                             true);
}

State & State::NewUser(void (* fn)()) {
  return *Domain::GetCurrent().New<x64::State>((uint64_t)fn, 0, false);
}

State & State::NewUser(void (* fn)(void *), void * arg) {
  return *Domain::GetCurrent().New<x64::State>((uint64_t)fn, (uint64_t)arg,
                                             false);
}

namespace x64 {

State::State(uint64_t rip, uint64_t _rdi, bool kernel) : rdi(_rdi) {
  iretInfo.rip = rip;
  iretInfo.rflags = 0x200;
  
  if (!Domain::GetCurrent().Alloc(stackStart, StackSize)) {
    Panic("State::State() - failed to allocate stack");
  }
  
  if (kernel) {
    iretInfo.rsp = (uint64_t)stackStart + StackSize;
    iretInfo.cs = 8;
    iretInfo.ss = 0;
  } else {
    iretInfo.cs = 0x23;
    iretInfo.ss = 0x1b;
  }
}

State::~State() {
  Domain::GetCurrent().Free(stackStart);
}

void * State::GetStackTop() {
  return (void *)((uint64_t)stackStart + StackSize);
}

void State::Delete() {
  Domain::GetCurrent().Delete(this);
}

void State::Resume() {
  AssertCritical();
  if (!iretInfo.ss) {
    Cpu::GetCurrent().SetAsyncKernelTop((void *)GetStackTop());
  }

  if (iretInfo.cs & 3) {
    __asm__ __volatile__("swapgs");
  }

  __asm__ __volatile__(
    "sub $0x28, %%rsp\n"
    "mov $5, %%rcx\n"
    "mov %%rsp, %%rdi\n"
    "rep movsq\n"
    "mov %%rdx, %%rdi\n"
    "iretq"
    : : "S" (&iretInfo), "d" (rdi)
  );
  
  __builtin_unreachable();
}

void State::SuspendAndCall(void (* func)()) {
  AssertCritical();
  Cpu & cpu = Cpu::GetCurrent();
  // we must be in kernel space, so we know the CS and SS
  iretInfo.cs = 8;
  iretInfo.ss = 0;
  __asm__ __volatile__(
    "movq $anarch_suspend_and_call_return, (%%rdi)\n" // save RIP
    "pushfq\n" // save RFLAGS
    "pop %%rax\n"
    "mov %%rax, 0x10(%%rdi)\n"
    "mov %%rsp, 0x18(%%rdi)\n" // save RSP
    "mov %%rcx, %%rsp\n" // load new RSP
    "call *%%rbx\n" // call func()
    "anarch_suspend_and_call_return:"
    : : "c" (cpu.GetStackTop()), "b" (func), "D" (&iretInfo)
    : "rax", "rdx", "rsi", "r8", "r9", "r10", "r11", "r12", "r13", "r14",
      "r15", "memory");
}

}

}
