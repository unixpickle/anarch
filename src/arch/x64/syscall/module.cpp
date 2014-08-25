#include "module.hpp"
#include "args.hpp"
#include "handler.hpp"
#include "../common.hpp"
#include "../interrupts/idt.hpp"
#include "../domains/domain-list.hpp"
#include <anarch/new>
#include <anarch/assert>
#include <anarch/critical>

namespace anarch {

SyscallModule & SyscallModule::GetGlobal() {
  return x64::SyscallModule::GetGlobal();
}

namespace x64 {

namespace {

SyscallModule gModule;

}

void SyscallModule::InitGlobal() {
  new(&gModule) SyscallModule();
}

SyscallModule & SyscallModule::GetGlobal() {
  return gModule;
}

void SyscallModule::SetHandler(SyscallHandler h) {
  assert(!handler);
  handler = h;
}

SyscallModule::SyscallHandler SyscallModule::GetHandler() {
  return handler;
}

ansa::DepList SyscallModule::GetDependencies() {
  return ansa::DepList(&DomainList::GetGlobal());
}

void SyscallModule::Initialize() {
  anarch::ScopedCritical critical;
  
  // send an IPI to each CPU to setup the syscall registers
  DomainList & domains = DomainList::GetGlobal();
  for (int i = 0; i < domains.GetCount(); ++i) {
    Domain & domain = domains[i];
    for (int j = 0; j < domain.GetThreadCount(); ++j) {
      Thread & th = domain.GetThread(j);
      if (&th == &Thread::GetCurrent()) continue;
      th.RunAsync(&SetCpuRegisters);
      ++initCount;
    }
  }
  ++initCount;
  SetCpuRegisters();
  while (initCount);
}

void SyscallModule::SetCpuRegisters() {
  // these are important because a SYSCALL handler sets the user-stack back
  // before returning and enabling interrupts.
  Idt::GetGlobal().SetIst(0x2, 1); // NMI
  Idt::GetGlobal().SetIst(0x12, 1); // MCE
  
  ScopedCritical critical;
  --(SyscallModule::GetGlobal().initCount);
  uint64_t star = (8L << 0x20) | (0x13L << 0x30);
  WriteMsr(MsrSTAR, star);
  WriteMsr(MsrSFMask, 0x200); // disable interrupts
  WriteMsr(MsrLSTAR, (uint64_t)AnarchRawSyscallHandler);
}

}

}

anarch::SyscallRet AnarchSyscallMainEntry(uint64_t a, uint64_t b, uint64_t c,
                                          uint64_t d, uint64_t e, uint64_t f) {
  AssertCritical();
  auto & module = anarch::x64::SyscallModule::GetGlobal();
  auto handler = module.GetHandler();
  assert(handler != NULL);
  anarch::x64::SyscallArgs args(b, c, d, e, f);
  return handler((uint16_t)a, args);
}
