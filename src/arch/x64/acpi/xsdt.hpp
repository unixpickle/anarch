#ifndef __ANARCH_X64_XSDT_HPP__
#define __ANARCH_X64_XSDT_HPP__

#include "acpi-root.hpp"

namespace anarch {

namespace x64 {

class Xsdt : public AcpiRoot {
public:
  Xsdt(PhysAddr phys);
  
  virtual int GetCount();
  virtual PhysAddr GetTable(int i);
};

}

}

#endif
