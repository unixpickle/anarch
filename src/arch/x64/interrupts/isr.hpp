#ifndef __ANARCH_X64_ISR_HPP__
#define __ANARCH_X64_ISR_HPP__

#include "isr-types.hpp"

extern "C" {

void InterruptCoded(uint64_t vector, anarch::x64::IsrStack * stack,
                    uint64_t code) __asm__("InterruptCoded");

void InterruptNonCoded(uint64_t vector, anarch::x64::IsrStack * stack)
  __asm__("InterruptNonCoded");

void InterruptPicEoi(uint64_t vector, anarch::x64::IsrStack * stack)
  __asm__("InterruptPicEoi");

}

#endif
