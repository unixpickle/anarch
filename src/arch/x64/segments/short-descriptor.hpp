#ifndef __ANARCH_X64_SHORT_DESCRIPTOR_HPP__
#define __ANARCH_X64_SHORT_DESCRIPTOR_HPP__

#include <anarch/stdint>
#include <ansa/macros>

namespace anarch {

namespace x64 {

struct ShortDescriptor {
  ShortDescriptor(bool privileged, bool executable);
  
  uint16_t limit;
  uint16_t baseLow;
  uint8_t baseMid;
  
  // access byte
  unsigned accessBit : 1;
  unsigned writable : 1;
  unsigned direction : 1;
  unsigned executable : 1;
  unsigned reservedOne : 1;
  unsigned privilege : 2;
  unsigned present : 1;
  
  unsigned limitHigh : 4;
  
  // flags half-byte
  unsigned reservedZero : 1;
  unsigned longMode : 1;
  unsigned size : 1;
  unsigned granularity : 1;
  
  uint8_t baseHigh;
} ANSA_PACKED;

static_assert(sizeof(ShortDescriptor) == 8, "Invalid short descriptor size");

}

}

#endif
