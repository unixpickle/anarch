#include "xapic.hpp"
#include "../../common.hpp"
#include <anarch/critical>

namespace anarch {

namespace x64 {

XApic::XApic(PhysAddr _base) : map(_base, 0x1000), base(_base) {
}

uint64_t XApic::ReadReg(uint16_t reg) {
  AssertCritical();
  volatile uint32_t * regs = (volatile uint32_t *)map.GetStart();
  if (reg != RegIcr) {
    return (uint64_t)regs[reg * 4];
  } else {
    uint32_t lower = regs[reg * 4];
    uint32_t higher = regs[(reg + 1) * 4];
    return (uint64_t)lower | ((uint64_t)higher << 0x20);
  }
}

void XApic::WriteReg(uint16_t reg, uint64_t value) {
  AssertCritical();
  volatile uint32_t * regs = (volatile uint32_t *)map.GetStart();
  if (reg != RegIcr) {
    assert(!(value & 0xFFFFFFFF00000000UL));
    regs[reg * 4] = value;
  } else {
    regs[(reg + 1) * 4] = value >> 0x20;
    regs[reg * 4] = value & 0xFFFFFFFF;
  }
}

void XApic::Enable() {
  AssertCritical();
  uint64_t flags = ReadMsr(0x1b) & 0xf00;
  flags |= 1 << 11;
  WriteMsr(0x1b, (uint64_t)base | flags);
}

uint32_t XApic::GetId() {
  AssertCritical();
  return ReadReg(RegApicId) >> 0x18;
}

void XApic::SendIpi(uint32_t cpu, uint8_t vector, uint8_t mode, uint8_t level,
                    uint8_t trigger) {
  AssertCritical();
  uint64_t value = 0;
  value = (uint64_t)vector | ((uint64_t)mode << 8);
  value |= ((uint64_t)level << 0xe) | ((uint64_t)trigger << 0xf);
  value |= ((uint64_t)cpu << 0x38);
  WriteReg(RegIcr, value);
}

}

}