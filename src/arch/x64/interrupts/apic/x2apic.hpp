#ifndef __ANARCH_X64_X2APIC_HPP__
#define __ANARCH_X64_X2APIC_HPP__

#include "lapic.hpp"

namespace anarch {

namespace x64 {

class X2Apic : public Lapic {
public:
  virtual uint64_t ReadReg(uint16_t reg);
  virtual void WriteReg(uint16_t reg, uint64_t value);
  virtual void Enable();
  virtual uint32_t GetId();
  virtual void SendIpi(uint32_t cpu, uint8_t vector, uint8_t mode,
                       uint8_t level, uint8_t trigger);
};

}

}

#endif