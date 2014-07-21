#ifndef __ANARCH_TEST_INVERTER_HPP__
#define __ANARCH_TEST_INVERTER_HPP__

#include <cstdint>

inline uint64_t InvertPhys(uint64_t value) {
  return value ^ 0xFFFFFFFFFFFF000UL;
}

#endif