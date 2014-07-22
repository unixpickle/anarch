#ifndef __ANARCH_X64_IOAPIC_HPP__
#define __ANARCH_X64_IOAPIC_HPP__

#include "../../acpi/apic-table.hpp"
#include <anarch/lock>
#include <ansa/nocopy>

namespace anarch {

namespace x64 {

class IOApic : public ansa::NoCopy {
public:
  struct Entry;
  static const int RegVersion = 1;
  
  IOApic(ApicTable & table, ApicTable::IOApic * info); // @noncritical
  
  // destructor is implicit, but is still @noncritical
  
  void WriteReg(uint8_t reg, uint32_t val); // @ambicritical
  uint32_t ReadReg(uint8_t reg); // @ambicritical
  
  uint32_t GetVersion(); // @ambicritical
  uint32_t GetPinCount(); // @ambicritical
  uint32_t GetInterruptBase(); // @ambicritical
  
  void SetEntry(uint8_t idx, const Entry & entry); // @ambicritical
  void MapIrq(uint8_t irq, uint8_t vector); // @ambicritical
  void MaskIrq(uint8_t irq); // @ambicritical
  void MaskPin(uint8_t irq); // @ambicritical
  
  struct Entry {
    unsigned vector : 8; // RW - processor register
    unsigned delmode : 3; // RW
    unsigned destmode : 1; // RW - determines type for destfield
    unsigned delstatus : 1; // RO
    unsigned intpol : 1; // RW - 0 = high active, 1 = low active
    unsigned remirr : 1; // RO
    unsigned triggermode : 1; // 1 = level sensitive, 0 = edge sensitive
    unsigned imask : 1; // 1 = prevent this interrupt
    unsigned long long reserved : 39; // set this to 0
    unsigned destfield : 8; // RW - APIC ID or "set of processors"
  } ANSA_PACKED;
  
private:
  ApicTable & table;
  ApicTable::IOApic info;
  EasyMap map;
  
  CriticalLock lock;
  CriticalLock tableLock;
};

}

}

#endif
