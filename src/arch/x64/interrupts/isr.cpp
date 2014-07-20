#include "isr.hpp"
#include "irt.hpp"
#include "../common.hpp"
#include <anarch/stream>
#include <anarch/api/panic>

typedef void (* RoutineCall)(anarch::x64::IsrStack * stack, uint64_t code);

extern "C" {

void InterruptCoded(uint64_t vector, anarch::x64::IsrStack * stack,
                    uint64_t code) {
  void * routine = anarch::x64::Irt::GetGlobal().Get((uint8_t)vector);
  if (routine) {
    ((RoutineCall)routine)(stack, code);
  } else {
    anarch::cout << "InterruptCoded(" << vector << ", " << (uint64_t)stack
      << ", " << code << ") - caller=" << stack->rip << anarch::endl;
    anarch::Panic("unhandled exception");
  }
}

void InterruptNonCoded(uint64_t vector, anarch::x64::IsrStack * stack) {
  void * routine = anarch::x64::Irt::GetGlobal().Get((uint8_t)vector);
  if (routine) {
    ((RoutineCall)routine)(stack, 0);
  } else {
    anarch::cout << "InterruptCoded(" << vector << ", " << (uint64_t)stack
      << ") - caller=" << stack->rip << anarch::endl;
    anarch::Panic("unhandled interrupt");
  }
}

void InterruptPicEoi(uint64_t vector, anarch::x64::IsrStack *) {
  if (vector > 0xf) {
    anarch::x64::OutB(0xa0, 0x20);
  }
  anarch::x64::OutB(0x20, 0x20);
}

}
