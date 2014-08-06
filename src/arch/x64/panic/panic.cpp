#include "panic.hpp"
#include "../interrupts/irt.hpp"
#include "../interrupts/vectors.hpp"
#include "../interrupts/apic/lapic-module.hpp"
#include "../domains/domain-list.hpp"
#include <anarch/api/panic>
#include <anarch/stdint>
#include <anarch/new>

namespace anarch {

void Panic(const char * msg) {
  x64::PanicModule::GetGlobal().Panic(msg);
}

namespace x64 {

namespace {

PanicModule gPanicModule;

}

void PanicModule::InitGlobal() {
  new(&gPanicModule) PanicModule();
}

PanicModule & PanicModule::GetGlobal() {
  return gPanicModule;
}

void PanicModule::Panic(const char * msg) {
  __asm__ __volatile__("cli");
  if (panicCount++) {
    __asm__ __volatile__("hlt");
  }
  
  // display the error now incase DistributeError() somehow fails
  WriteError(msg);
  DistributeError();
  
  // display the error again incase other CPUs up to this point were messing
  // with the display buffer
  WriteError(msg);
  
  __asm__ __volatile__("hlt");
  __builtin_unreachable();
}

void PanicModule::Initialize() {
  Irt::GetGlobal().Set(IntVectors::Panic, (void *)HaltCpu);
}

ansa::DepList PanicModule::GetDependencies() {
  return ansa::DepList(&Irt::GetGlobal(), &LapicModule::GetGlobal(),
                       &DomainList::GetGlobal());
}

void PanicModule::DistributeError() {
  if (!IsInitialized()) return;
  Lapic & lapic = LapicModule::GetGlobal().GetLapic();
  Cpu * current = &Cpu::GetCurrent();
  DomainList & domains = DomainList::GetGlobal();
  for (int i = 0; i < domains.GetCount(); i++) {
    Domain & domain = domains[i];
    for (int j = 0; j < domain.GetThreadCount(); j++) {
      Cpu * cpu = &domain.GetCpu(j);
      if (cpu == current) continue;
      lapic.SendIpi(cpu->GetApicId(), IntVectors::Panic);
    }
  }
}

void PanicModule::WriteError(const char * msg) {
  // write the error to the screen
  uint16_t * buf = (uint16_t *)0xb8000;
  int i = 0;
  const char * prefix = "PANIC: ";
  while (*prefix) {
    buf[i++] = *prefix | 0xf200;
    prefix++;
  }
  while (*msg) {
    buf[i++] = *msg | 0xf900;
    msg++;
  }
  buf[i] = 0xf920; // space at the end for aesthetics
}

void PanicModule::HaltCpu() {
  // should I even bother sending an EOI, like really
  LapicModule::GetGlobal().GetLapic().SendEoi();
  while (1) {
    __asm__ __volatile__("cli\nhlt");
  }
}

}

}