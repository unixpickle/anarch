#ifndef __ANARCH_X64_IDT_TYPES_HPP__
#define __ANARCH_X64_IDT_TYPES_HPP__

#include <anarch/stdint>
#include <ansa/macros>

namespace anarch {

namespace x64 {

/**
 * Represents this x86-64 machine code:
 * 
 * pushq %rax
 * pushq %rax
 * movabsq $argument, %rax
 * movq %rax, 0x8(%rsp)
 * movabsq $function, %rax
 * xchgq %rax, (%rsp)
 * ret
 */
struct IdtHandler {
  unsigned char code1[4] {0x50, 0x50, 0x48, 0xB8};
  uint64_t argument = 0;
  unsigned char code2[7] {0x48, 0x89, 0x44, 0x24, 0x08, 0x48, 0xB8};
  uint64_t function = 0;
  unsigned char code3[5] {0x48, 0x87, 0x04, 0x24, 0xc3};
} ANSA_PACKED;

static_assert(sizeof(IdtHandler) == 0x20, "IdtHandler size == 0x20");

struct IdtEntry {
  uint16_t lowOffset = 0;
  uint16_t codeSegment = 8;
  uint8_t ist : 3;
  uint8_t reserved1 : 5;
  uint8_t flags = 0;
  uint16_t midOffset = 0;
  uint32_t highOffset = 0;
  uint32_t reserved2 = 0;
 
  IdtEntry() : ist(0), reserved1(0) {}
 
  void SetOffset(uint64_t exc) {
    lowOffset = exc & 0xffff;
    midOffset = (exc >> 16) & 0xffff;
    highOffset = exc >> 32;
  }
} ANSA_PACKED;

static_assert(sizeof(IdtEntry) == 0x10, "IdtEntry size == 0x10");

struct IdtPointer {
  uint16_t limit;
  uint64_t virtualAddress;
} ANSA_PACKED;

static_assert(sizeof(IdtPointer) == 0x0a, "IdtPointer size == 0x0a");

}

}

#endif
