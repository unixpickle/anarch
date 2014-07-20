#include "isr.hpp"
#include <anarch/stream>

extern "C" {

void InterruptCoded(uint64_t vector, anarch::x64::IsrStack * stack,
                    uint64_t code) {
  anarch::cout << "InterruptCoded(" << vector << ", " << (uint64_t)stack
    << ", " << code << ")" << anarch::endl;
}

void InterruptNonCoded(uint64_t vector, anarch::x64::IsrStack * stack) {
  anarch::cout << "InterruptNonCoded(" << vector << ", " << (uint64_t)stack
    << ")" << anarch::endl;
}

void InterruptPicEoi(uint64_t vector, anarch::x64::IsrStack * stack) {
  anarch::cout << "InterruptPicEoi(" << vector << ", " << (uint64_t)stack
    << ")" << anarch::endl;
}

}
