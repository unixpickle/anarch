#include "hpet.hpp"
#include <anarch/critical>

namespace anarch {

namespace x64 {

namespace {

uint64_t ExtractTicksPerMin(EasyMap & map) {
  volatile uint64_t * regs = (volatile uint64_t *)map.GetStart();
  return 60000000000000000L / (regs[0] >> 0x20);
}

}

Hpet::Hpet(const HpetTable & info)
  : map(info.address.address, 0x1000),
    microsPerTick(60000000, ExtractTicksPerMin(map)) {      
}

void Hpet::Start() {
  AssertNoncritical();
  ScopedCritical critical;
  volatile uint64_t * regs = (volatile uint64_t *)map.GetStart();
  regs[2] |= 1;
}

void Hpet::Stop() {
  AssertNoncritical();
  ScopedCritical critical;
  volatile uint64_t * regs = (volatile uint64_t *)map.GetStart();
  regs[2] &= ~(uint64_t)1;
}

uint64_t Hpet::GetTicks() {
  volatile uint64_t * regs = (volatile uint64_t *)map.GetStart();
  return regs[0x1e];
}

ansa::Rational<uint64_t> Hpet::GetMicrosPerTick() {
  return microsPerTick;
}

}

}
