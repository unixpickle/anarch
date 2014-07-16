#ifndef __ANARCH_X64_INIT_HPP__
#define __ANARCH_X64_INIT_HPP__

#include "boot-info.hpp"

namespace anarch {

namespace x64 {

void InitializeSingletons();
void SetBootInfo(const BootInfo * info);
const BootInfo * GetBootInfo();

}

}

#endif
