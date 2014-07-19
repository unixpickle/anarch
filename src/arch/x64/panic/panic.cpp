#include "panic.hpp"
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
  // TODO: here, register the Panic IPI, etc.
}

ansa::DepList PanicModule::GetDependencies() {
  // TODO: here, we return whatever module will initialize the CPU list
  return ansa::DepList();
}

void PanicModule::DistributeError() {
  if (!IsInitialized()) return;
  // TODO: here, use IPIs to halt other CPUs
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

}

}