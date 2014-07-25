#include "local-segment.hpp"
#include "../common.hpp"
#include <anarch/critical>

namespace anarch {

namespace x64 {

void LocalSegment::Swap() {
  AssertCritical();
  __asm__ __volatile__("swapgs");
}

uint64_t LocalSegment::Read() {
  AssertCritical();
  return ReadMsr(0xc0000101);
}

void LocalSegment::Write(uint64_t value) {
  AssertCritical();
  WriteMsr(0xc0000101, value);
}

LocalSegment::SwapScope::SwapScope(uint8_t vector, const IsrStack & stack) {
  if (vector != 0x2 && vector != 0x12) {
    didSwap = (stack.cs & 3) != 0;
  } else {
    // TODO: here, use Task Register to identify the CPU
    didSwap = (LocalSegment::Read() == 0);
  }
  if (didSwap) {
    LocalSegment::Swap();
  }
}

LocalSegment::SwapScope::~SwapScope() {
  if (didSwap) {
    LocalSegment::Swap();
  }
}

}

}
