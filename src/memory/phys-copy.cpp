#include "phys-copy.hpp"
#include "easy-map.hpp"
#include <ansa/cstring>

namespace anarch {

void PhysCopy(void * dest, PhysAddr source, size_t size) {
  EasyMap map(source, (PhysSize)size);
  ansa::Memcpy(dest, (uint8_t *)map.GetStart(), size);
}

}
