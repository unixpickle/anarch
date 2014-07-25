#ifndef __ANARCH_X64_TSS_HPP__
#define __ANARCH_X64_TSS_HPP__

#include <anarch/stdint>
#include <ansa/macros>

namespace anarch {

namespace x64 {

struct Tss {
  uint32_t res0;
  uint64_t rsp[3];
  uint64_t res1;
  uint64_t ist[7];
  uint64_t res2;
  uint16_t res3;
  uint16_t ioplBase;
} ANSA_PACKED;

struct TssDescriptor {
  uint16_t limit_0;
  uint16_t base_0;
  uint8_t base_16;
  unsigned type : 4;
  unsigned res0 : 1;
  unsigned dpl : 2;
  unsigned present : 1;
  unsigned limit_16 : 4;
  unsigned available : 1;
  unsigned res1 : 2;
  unsigned granularity : 1;
  uint8_t base_24;
  uint32_t base_32;
  uint32_t res2;

  TssDescriptor(Tss * tss);
  Tss * GetPointer();
} ANSA_PACKED;

static_assert(sizeof(TssDescriptor) == 16, "Invalid TSS descriptor size");

}

}

#endif
