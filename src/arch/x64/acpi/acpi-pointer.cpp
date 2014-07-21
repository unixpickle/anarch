#include "acpi-pointer.hpp"
#include "rsdt.hpp"
#include "xsdt.hpp"
#include "../vmm/global/global-malloc.hpp"
#include <ansa/cstring>

namespace anarch {

namespace x64 {

namespace {

uint8_t MemChecksum(uint8_t * ptr, int len) {
  uint8_t sum = 0;
  while (len-- > 0) {
    sum += (*ptr);
    ptr++;
  }
  return sum;
}

}

AcpiRoot * AcpiPointer::GenerateRoot() {
  VirtualAllocator & allocator = GlobalMalloc::GetGlobal().GetAllocator();
  if (revision == 0) {
    return allocator.New<Rsdt>((PhysAddr)ptrRSDT);
  } else {
    return allocator.New<Xsdt>((PhysAddr)ptrXSDT);
  }
}

AcpiPointer * AcpiPointer::Find() {
  // find the RSDP in the BIOS areas.
  const char * signature = "RSD PTR ";
  uintptr_t ptr;
  
  // the whole potential EBDA
  for (ptr = 0x80000; ptr < 0x9fc00; ptr++) {
    if (ansa::Memcmp(signature, (void *)ptr, 8) == 0) {
      if (MemChecksum((uint8_t *)ptr, 0x14) != 0) continue;
      return (AcpiPointer *)ptr;
    }
  }
  for (ptr = 0xe0000; ptr < 0x100000; ptr++) {
    if (ansa::Memcmp(signature, (void *)ptr, 8) == 0) {
      if (MemChecksum((uint8_t *)ptr, 0x14) != 0) continue;
      return (AcpiPointer *)ptr;
    }
  }
  
  return NULL;
}

}

}
