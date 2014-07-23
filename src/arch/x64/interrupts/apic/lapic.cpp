#include "lapic.hpp"
#include <anarch/critical>

namespace anarch {

namespace x64 {

void Lapic::SetDefaults() {
  AssertCritical();
  WriteReg(RegTaskPriority, 0x0);
  WriteReg(RegLvtTimer, 0x10000);
  WriteReg(RegLvtPerf, 0x10000);
  WriteReg(RegLvtLint0, 0x8700);
  WriteReg(RegLvtLint1, 0x400);
  WriteReg(RegLvtError, 0x10000);
  WriteReg(RegSpurious, 0x1ff);

  // reset might have shut them off
  WriteReg(RegLvtLint0, 0x8700);
  WriteReg(RegLvtLint1, 0x400);

  WriteReg(RegTimerDiv, PreferredDivide);
}

void Lapic::ClearErrors() {
  AssertCritical();
  WriteReg(RegEsr, 0);
}

void Lapic::SendEoi() {
  AssertCritical();
  WriteReg(RegEoi, 0);
}

void Lapic::SetPriority(uint8_t vector) {
  AssertCritical();
  WriteReg(RegTaskPriority, vector);
}

bool Lapic::IsRequested(uint8_t vector) {
  AssertCritical();
  uint64_t regIndex = 0x20 + (vector >> 5);
  uint32_t mask = (1 << (vector & 0x1f));
  return 0 != (ReadReg(regIndex) & mask);
}

bool Lapic::IsInService(uint8_t vector) {
  AssertCritical();
  uint64_t regIndex = 0x10 + (vector >> 5);
  uint32_t mask = (1 << (vector & 0x1f));
  return 0 != (ReadReg(regIndex) & mask);
}

void Lapic::SetTimeout(uint8_t vector, uint32_t ticks) {
  uint32_t timerField = vector;// | (2 << 17); // mode is bit 17
  WriteReg(RegLvtTimer, timerField);
  WriteReg(RegTimerInitCount, ticks);
}

void Lapic::ClearTimeout() {
  WriteReg(RegLvtTimer, 0x10000);
}

}

}
