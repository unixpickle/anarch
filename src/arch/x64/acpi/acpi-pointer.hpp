#ifndef __ANARCH_ACPI_POINTER_HPP__
#define __ANARCH_ACPI_POINTER_HPP__

#include "acpi-root.hpp"
#include <anarch/stdint>
#include <ansa/macros>

namespace anarch {

namespace x64 {

// this structure is also known as the RSDP (or XSDP)
class AcpiPointer {
public:
  uint64_t signature;
  uint8_t checksum;
  char oemid[6];
  uint8_t revision;
  uint32_t ptrRSDT;
  uint32_t length;
  uint64_t ptrXSDT;
  uint8_t xChecksum;
  char reserved[3];
  
  AcpiRoot * GenerateRoot();
  
  static AcpiPointer * Find();
} ANSA_PACKED;

}

}

#endif
