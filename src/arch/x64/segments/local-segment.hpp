#ifndef __ANARCH_X64_LOCAL_SEGMENT_HPP__
#define __ANARCH_X64_LOCAL_SEGMENT_HPP__

#include "../interrupts/isr-types.hpp"

namespace anarch {

namespace x64 {

class LocalSegment {
public:
  static void Swap(); // @critical
  static uint64_t Read(); // @critical
  static void Write(uint64_t); // @critical
  
  class SwapScope {
  public:
    SwapScope(uint8_t vector, const IsrStack & stack);
    ~SwapScope();
  private:
    bool didSwap;
  };
};

}

}

#endif
