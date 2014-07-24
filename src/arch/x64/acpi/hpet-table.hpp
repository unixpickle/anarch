#ifndef __ANARCH_X64_HPET_TABLE_HPP__
#define __ANARCH_X64_HPET_TABLE_HPP__

#include <anarch/stdint>
#include <ansa/macros>

namespace anarch {

namespace x64 {

struct HpetTable {
  struct Address {
    uint8_t addressSpaceId;
    uint8_t registerBitWidth;
    uint8_t registerBitOffset;
    uint8_t reserved;
    uint64_t address;
  } ANSA_PACKED;
  
  uint32_t signature; // 'HPET'
  uint32_t length;
  uint8_t revision;
  uint8_t checksum;
  char oemid[6];
  uint64_t oemTableId;
  uint32_t oemRevision;
  uint32_t creatorId;
  uint32_t creatorRevision;
  
  uint32_t blockID;
  Address address;
  uint8_t hpetNumber;
  uint16_t counterMinimum;
  uint8_t pageProtectionAndOEM;
};

}

}

#endif
