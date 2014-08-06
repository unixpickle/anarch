#ifndef __ANARCH_X64_ISR_TYPES_HPP__
#define __ANARCH_X64_ISR_TYPES_HPP__

#include <anarch/stdint>
#include <ansa/macros>

namespace anarch {

namespace x64 {

struct IsrStack {
  uint64_t rip;
  uint64_t cs;
  uint64_t rflags;
  uint64_t rsp;
  uint64_t ss;
};

static_assert(sizeof(IsrStack) == 0x28, "invalid IRETQ stack size");

}

}

#endif
