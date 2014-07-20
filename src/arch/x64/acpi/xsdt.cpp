#include "xsdt.hpp"

namespace anarch {

namespace x64 {

Xsdt::Xsdt(PhysAddr phys) : AcpiRoot(phys) {
}

int Xsdt::GetCount() {
  return (int)((tableSize - 0x24) / 8);
}

PhysAddr Xsdt::GetTable(int i) {
  uint64_t * ptr = (uint64_t *)(map->GetStart() + 0x24);
  return ptr[i];
}

}

}
