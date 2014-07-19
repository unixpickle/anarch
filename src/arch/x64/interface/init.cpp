#include "init.hpp"
#include "../vmm/tlb.hpp"
#include "../vmm/global/global-map.hpp"
#include "../console/text-console.hpp"
#include "../panic/panic.hpp"
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
  TLB::InitGlobal();
  TextConsole::InitGlobal();
  PanicModule::InitGlobal();
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
