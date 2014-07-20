#ifndef __ANARCH_X64_RSDT_HPP__
#define __ANARCH_X64_RSDT_HPP__

#include "acpi-root.hpp"

namespace anarch {

namespace x64 {

class Rsdt : public AcpiRoot {
public:
  Rsdt(PhysAddr phys);
  
  virtual int GetCount();
  virtual PhysAddr GetTable(int i);
};

}

}

#endif
