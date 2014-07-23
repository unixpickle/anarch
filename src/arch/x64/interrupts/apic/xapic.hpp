#ifndef __ANARCH_X64_XAPIC_HPP__
#define __ANARCH_X64_XAPIC_HPP__

#include "lapic.hpp"
#include <anarch/easy-map>
#include <anarch/types>

namespace anarch {

namespace x64 {

class XApic : public Lapic {
public:
  XApic(PhysAddr base); // @noncritical
  
  // destructor is not listed but is also @noncritical
  
  virtual uint64_t ReadReg(uint16_t reg);
  virtual void WriteReg(uint16_t reg, uint64_t value);
  virtual void Enable();
  virtual uint32_t GetId();
  virtual void SendIpi(uint32_t cpu, uint8_t vector, uint8_t mode,
                       uint8_t level, uint8_t trigger);

private:
  EasyMap map;
  PhysAddr base;
};

}

}

#endif
