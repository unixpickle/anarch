#include "init.hpp"
#include "../vmm/tlb.hpp"
#include "../vmm/global/global-malloc.hpp"
#include "../vmm/global/global-map.hpp"
#include "../interrupts/idt.hpp"
#include "../interrupts/irt.hpp"
#include "../interrupts/pic.hpp"
#include "../interrupts/apic/ioapic-module.hpp"
#include "../interrupts/apic/lapic-module.hpp"
#include "../console/text-console.hpp"
#include "../domains/domain-list.hpp"
#include "../acpi/acpi-module.hpp"
#include "../critical/module.hpp"
#include "../syscall/module.hpp"
#include "../segments/gdt.hpp"
#include "../timer/module.hpp"
#include "../clock/module.hpp"
#include "../panic/panic.hpp"
#include <anarch/stream>
#include <anarch/assert>
#include <anarch/stddef>

namespace anarch {

namespace x64 {

namespace {

const BootInfo * bootInfo = NULL;

}

void InitializeSingletons() {
  bootInfo = NULL;
  CriticalModule::InitGlobal();
  SyscallModule::InitGlobal();
  GlobalMalloc::InitGlobal();
  IOApicModule::InitGlobal();
  TextConsole::InitGlobal();
  PanicModule::InitGlobal();
  LapicModule::InitGlobal();
  TimerModule::InitGlobal();
  ClockModule::InitGlobal();
  DomainList::InitGlobal();
  AcpiModule::InitGlobal();
  GlobalMap::InitGlobal();
  Tlb::InitGlobal();
  Idt::InitGlobal();
  Irt::InitGlobal();
  Pic::InitGlobal();
  Gdt::InitGlobal();
  
  StreamModule::InitGlobal();
}

void SetBootInfo(const BootInfo * info) {
  assert(info != NULL);
  assert(!bootInfo);
  bootInfo = info;
}

const BootInfo * GetBootInfo() {
  return bootInfo;
}

}

}
