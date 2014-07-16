#include "init.hpp"
#include <anarch/assert>

namespace anarch {

namespace x64 {

namespace {

const BootInfo * bootInfo;

}

void InitializeSingletons() {
  // TODO: here, there are going to be lots of architecture-specific singletons
  // to initialize
  bootInfo = NULL;
}

void SetBootInfo(const BootInfo * info) {
  assert(info);
  assert(!bootInfo);
  bootInfo = info;
}

const BootInfo * GetBootInfo() {
  return bootInfo;
}

}

}
