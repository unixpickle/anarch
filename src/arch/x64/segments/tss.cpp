#include "tss.hpp"

namespace anarch {

namespace x64 {

TssDescriptor::TssDescriptor(Tss * tss)
  : limit_0(0x67), type(9), res0(0), dpl(0), present(1), limit_16(0),
    available(1), res1(0), granularity(0), res2(0) {
  // as much as I love initializer lists, I like code too.
  uint64_t ptrVal = (uint64_t)tss;
  base_0 = ptrVal & 0xffff;
  base_16 = (ptrVal >> 16) & 0xff;
  base_24 = (ptrVal >> 24) & 0xff;
  base_32 = ptrVal >> 0x20;
}


Tss * TssDescriptor::GetPointer() {
  uint64_t ptrVal = 0;
  ptrVal |= base_0;
  ptrVal |= base_16 << 16;
  ptrVal |= base_24 << 24;
  ptrVal |= (uint64_t)base_32 << 0x20;
  return (Tss *)ptrVal;
}

}

}
