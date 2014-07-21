#include "ioapic.hpp"
#include <anarch/critical>
#include <ansa/cstring>

namespace anarch {

namespace x64 {

IOApic::IOApic(ApicTable & _table, ApicTable::IOApic * _info)
  : table(_table), info(*_info), map((PhysAddr)info.baseAddr, 20) {
}

void IOApic::WriteReg(uint8_t reg, uint32_t val) {
  ScopedCritical critical;
  ScopedLock scope(lock);
  
  volatile uint32_t * regs = (volatile uint32_t *)map.GetStart();
  regs[0] = reg;
  regs[4] = val;
}

uint32_t IOApic::ReadReg(uint8_t reg) {
  ScopedCritical critical;
  ScopedLock scope(lock);
  
  volatile uint32_t * regs = (volatile uint32_t *)map.GetStart();
  regs[0] = reg;
  return regs[4];
}

uint32_t IOApic::GetVersion() {
  return ReadReg(RegVersion);
}

uint32_t IOApic::GetPinCount() {
  return 1 + ((ReadReg(RegVersion) >> 0x10) & 0xff);
}

uint32_t IOApic::GetInterruptBase() {
  return info.interruptBase;
}

void IOApic::SetEntry(uint8_t idx, const Entry & entry) {
  ScopedCritical critical;
  ScopedLock scope(tableLock);
  
  const uint32_t * valPtr = (const uint32_t *)&entry;
  WriteReg(0x10 + (idx * 2) , 0x10000); // mask the entry
  WriteReg(0x11 + (idx * 2), valPtr[1]);
  WriteReg(0x10 + (idx * 2), valPtr[0]);
}

void IOApic::MapIrq(uint8_t irq, uint8_t vector) {
  Entry entry;
  ansa::Bzero(&entry, sizeof(entry));
  entry.vector = vector;
  
  ApicTable::Iso * iso = table.LookupIso(irq);
  if (iso) {
    if (iso->flags & 0x3) entry.intpol = 1;
    if ((iso->flags >> 2) & 0x3) entry.triggermode = 1;
    SetEntry(iso->interrupt, entry);
  } else {
    SetEntry(irq, entry);
  }
}

void IOApic::MaskIrq(uint8_t irq) {
  ApicTable::Iso * iso = table.LookupIso(irq);
  if (iso) {
    MaskPin(iso->interrupt);
  } else {
    MaskPin(irq);
  }
}

void IOApic::MaskPin(uint8_t irq) {
  ScopedCritical critical;
  ScopedLock scope(tableLock);
  
  WriteReg(0x10 + (irq * 2), 0x10000);
}

}

}
