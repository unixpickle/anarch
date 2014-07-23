#include "x2apic.hpp"
#include "../../common.hpp"
#include <anarch/critical>

namespace anarch {

namespace x64 {

uint64_t X2Apic::ReadReg(uint16_t reg) {
  AssertCritical();
  return ReadMsr((uint32_t)reg + 0x800);
}

void X2Apic::WriteReg(uint16_t reg, uint64_t value) {
  AssertCritical();
  WriteMsr((uint32_t)reg + 0x800, value);
}

void X2Apic::Enable() {
  AssertCritical();
  uint64_t flags = ReadMsr(0x1b) & 0xf00;
  flags |= 3 << 10;
  WriteMsr(0x1b, flags);
}

uint32_t X2Apic::GetId() {
  AssertCritical();
  return ReadReg(RegApicId);
}

void X2Apic::SendIpi(uint32_t cpu, uint8_t vector, uint8_t mode, uint8_t level,
                     uint8_t trigger) {
  AssertCritical();
  uint64_t value = 0;
  value = (uint64_t)vector | ((uint64_t)mode << 8);
  value |= ((uint64_t)level << 0xe) | ((uint64_t)trigger << 0xf);
  value |= ((uint64_t)cpu << 0x20);
  WriteReg(RegIcr, value);
}

}

}
