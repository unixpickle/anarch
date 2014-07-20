#include "rsdt.hpp"

namespace anarch {

namespace x64 {

Rsdt::Rsdt(PhysAddr phys) : AcpiRoot(phys) {
}

int Rsdt::GetCount() {
  return (int)((tableSize - 0x24) / 4);
}

PhysAddr Rsdt::GetTable(int i) {
  uint32_t * ptr = (uint32_t *)(map->GetStart() + 0x24);
  return (PhysAddr)ptr[i];
}

}

}
