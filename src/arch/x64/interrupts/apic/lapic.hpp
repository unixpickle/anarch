#ifndef __ANARCH_X64_LAPIC_HPP__
#define __ANARCH_X64_LAPIC_HPP__

#include <anarch/stdint>

namespace anarch {

namespace x64 {

class Lapic {
public:
  static const int PreferredDivide = 0x3;
  
  virtual void SetDefaults(); // @critical
  virtual void Enable() = 0; // @critical
  virtual uint32_t GetId() = 0; // @critical
  virtual void ClearErrors(); // @critical

  virtual void SendEoi(); // @critical
  virtual void SetPriority(uint8_t vector);
  virtual bool IsRequested(uint8_t vector); // @critical
  virtual bool IsInService(uint8_t vector); // @critical

  virtual void SetTimeout(uint8_t vector, uint32_t ticks); // @critical
  virtual void ClearTimeout();

  /**
   * @critical
   */
  virtual void SendIpi(uint32_t cpu, uint8_t vector,
                       uint8_t mode = 0, uint8_t level = 1,
                       uint8_t trigger = 0) = 0;

  virtual uint64_t ReadReg(uint16_t reg) = 0; // @critical
  virtual void WriteReg(uint16_t reg, uint64_t value) = 0; // @critical
  
  enum Regs {
    RegApicId = 0x2,
    RegApicVer = 0x3,
    RegTaskPriority = 0x8,
    RegEoi = 0xb,
    RegSpurious = 0xf,
    RegEsr = 0x28,
    RegIcr = 0x30,
    RegLvtTimer = 0x32,
    RegLvtPerf = 0x34,
    RegLvtLint0 = 0x35,
    RegLvtLint1 = 0x36,
    RegLvtError = 0x37,
    RegTimerInitCount = 0x38,
    RegTimerCurrCount = 0x39,
    RegTimerDiv = 0x3e
  };
};

}

}

#endif
