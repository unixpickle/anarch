#include "init.hpp"
#include "../vmm/tlb.hpp"
#include "../vmm/global/global-map.hpp"
#include "../vmm/global/global-malloc.hpp"
#include "../interrupts/idt.hpp"
#include "../interrupts/irt.hpp"
#include "../interrupts/pic.hpp"
#include "../interrupts/apic/ioapic-module.hpp"
#include "../interrupts/apic/lapic-module.hpp"
#include "../console/text-console.hpp"
#include "../panic/panic.hpp"
#include "../acpi/acpi-module.hpp"
#include "../domains/domain-list.hpp"
#include "../critical/module.hpp"
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
  GlobalMap::InitGlobal();
  GlobalMalloc::InitGlobal();
  TLB::InitGlobal();
  TextConsole::InitGlobal();
  PanicModule::InitGlobal();
  AcpiModule::InitGlobal();
  Idt::InitGlobal();
  Irt::InitGlobal();
  Pic::InitGlobal();
  IOApicModule::InitGlobal();
  LapicModule::InitGlobal();
  DomainList::InitGlobal();
  CriticalModule::InitGlobal();
  
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
