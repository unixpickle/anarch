#include "acpi-root.hpp"
#include <anarch/phys-copy>

namespace anarch {

namespace x64 {

namespace {

uint32_t ReadPhysSize(PhysAddr addr) {
  uint32_t res;
  PhysCopy(&res, addr, 4);
  return res;
}

}

AcpiRoot::AcpiRoot(PhysAddr tableBase)
  : tableSize(ReadPhysSize(tableBase + 4)),
    map(tableBase, tableSize) {
}

int AcpiRoot::FindTable(const char * name) {
  uint32_t num = *((uint32_t *)name);
  int count = GetCount();
  for (int i = 0; i < count; i++) {
    PhysAddr tableAddr = GetTable(i);
    uint32_t typeNum;
    PhysCopy(&typeNum, tableAddr, 4);
    if (typeNum == num) {
      return i;
    }
  }
  return -1;
}

}

}
