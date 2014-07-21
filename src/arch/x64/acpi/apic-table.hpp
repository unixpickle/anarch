#ifndef __ANARCH_X64_ACPI_APIC_TABLE_HPP__
#define __ANARCH_X64_ACPI_APIC_TABLE_HPP__

#include <anarch/easy-map>
#include <anarch/stdint>
#include <anarch/stddef>
#include <anarch/types>
#include <ansa/macros>

namespace anarch {

namespace x64 {

class ApicTable {
public:
  struct Header;
  struct ISO;
  struct LocalAPIC;
  struct LocalAPIC2;
  struct IOAPIC;
  class Iterator;
  
  static const uint8_t TypeLAPIC = 0;
  static const uint8_t TypeIOAPIC = 1;
  static const uint8_t TypeISO = 2;
  static const uint8_t TypeX2APIC = 9;
  
  ApicTable(PhysAddr baseAddr); // @noncritical
  
  // destructor is implicit, but is @noncritical
  
  bool CanIterate(); // @ambicritical
  Iterator GetIterator(); // @ambicritical
  
  Header & GetHeader();
  
  bool SystemHas8259(); // @ambicritical
  int CountType(uint8_t type); // @ambicritical
  int CountIOAPICs(); // @ambicritical
  int CountLAPICs(bool checkUsable); // @ambicritical
  
  ISO * LookupISO(uint8_t physIRQ); // @ambicritical
  IOAPIC * LookupIOAPIC(uint32_t base); // @ambicritical
  
  struct Header {
    uint32_t signature; // APIC
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemid[6];
    uint64_t oemTableId;
    uint32_t oemRevision;
    uint32_t creatorId;
    uint32_t creatorRevision;
    uint32_t lapicAddr;
    uint32_t flags;
  } ANSA_PACKED;
  
  struct ISO {
    uint8_t type;
    uint8_t length;
    uint8_t bus;
    uint8_t source;
    uint32_t interrupt;
    uint16_t flags;
  } ANSA_PACKED;

  struct LocalAPIC {
    uint8_t type; // should be 0
    uint8_t length;
    uint8_t apicpuId;
    uint8_t apicId;
    uint32_t flags; // bit 0 set = usable
  } ANSA_PACKED;

  struct LocalAPIC2 {
    uint8_t type; // should be 9
    uint8_t length;
    uint16_t reserved;
    uint32_t x2apicId;
    uint32_t flags; // bit 0 set = usable
    uint32_t x2apicpuId;
  } ANSA_PACKED;

  struct IOAPIC {
    uint8_t type;
    uint8_t length;
    uint8_t identifier;
    uint8_t reserved;
    uint32_t baseAddr;
    uint32_t interruptBase;
  } ANSA_PACKED;
  
  class Iterator {
  public:
    bool Next();
    uint8_t GetType();
    void * GetData();
    size_t GetSize();
  protected:
    friend class ApicTable;
    Iterator(void * baseAddr, size_t dataSize);
    
  private:
    uint8_t * address;
    size_t dataSize;
  };
  
protected:
  PhysSize dataSize;
  EasyMap map;
  
  void * GetData();
  size_t GetDataSize();
};

}

}

#endif
