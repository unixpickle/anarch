#include "init.hpp"
#include "../vmm/tlb.hpp"
#include "../vmm/global/global-map.hpp"
#include "../vmm/global/global-malloc.hpp"
#include "../console/text-console.hpp"
#include "../panic/panic.hpp"
#include "../acpi/acpi-module.hpp"
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
