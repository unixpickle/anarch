#include "apic-table.hpp"
#include <anarch/phys-copy>
#include <anarch/assert>

namespace anarch {

namespace x64 {

namespace {

uint32_t ReadLength(PhysAddr addr) {
  uint32_t value;
  PhysCopy(&value, addr, 4);
  return value;
}

}

ApicTable::ApicTable(PhysAddr baseAddr)
  : dataSize((PhysSize)ReadLength(baseAddr + 4)),
    map(baseAddr, dataSize) {
}

bool ApicTable::CanIterate() {
  return GetDataSize() > 0;
}

ApicTable::Iterator ApicTable::GetIterator() {
  return Iterator((void *)GetData(), GetDataSize());
}

ApicTable::Header & ApicTable::GetHeader() {
  return *((Header *)map.GetStart());
}

bool ApicTable::SystemHas8259() {
  return (GetHeader().flags & 1) != 0;
}

int ApicTable::CountType(uint8_t type) {
  if (!CanIterate()) return 0;
  int count = 0;
  Iterator it = GetIterator();
  do {
    if (it.GetType() == type) count++;
  } while (it.Next());
  return count;
}

int ApicTable::CountIOAPICs() {
  return CountType(TypeIOAPIC);
}

int ApicTable::CountLAPICs(bool checkUsable) {
  if (!CanIterate()) return 0;
  
  int count = 0;
  Iterator it = GetIterator();
  do {
    if (it.GetType() == TypeLAPIC) {
      if (!checkUsable) {
        count++;
      } else {
        LocalAPIC * apic = (LocalAPIC *)it.GetData();
        if (apic->flags & 1) count++;
      }
    } else if (it.GetType() == TypeX2APIC) {
      if (!checkUsable) {
        count++;
      } else {
        LocalAPIC2 * apic = (LocalAPIC2 *)it.GetData();
        if (apic->flags & 1) count++;
      }
    }
  } while (it.Next());
  
  return count;
}

ApicTable::ISO * ApicTable::LookupISO(uint8_t physIRQ) {
  if (!CanIterate()) return NULL;
  Iterator it = GetIterator();
  do {
    if (it.GetType() != TypeISO) continue;
    ISO * iso = (ISO *)it.GetData();
    if (iso->source == physIRQ && iso->bus == 0) {
      return iso;
    }
  } while (it.Next());
  return NULL;
}

ApicTable::IOAPIC * ApicTable::LookupIOAPIC(uint32_t base) {
  if (!CanIterate()) return NULL;
  Iterator it = GetIterator();
  do {
    if (it.GetType() != TypeIOAPIC) continue;
    IOAPIC * info = (IOAPIC *)it.GetData();
    if (info->interruptBase == base) return info;
  } while (it.Next());
  return NULL;
}

void * ApicTable::GetData() {
  return (void *)(map.GetStart() + sizeof(Header));
}

size_t ApicTable::GetDataSize() {
  return (size_t)(dataSize - sizeof(Header));
}

// ITERATOR //

bool ApicTable::Iterator::Next() {
  uint8_t thisSize = GetSize();
  
  assert(thisSize <= dataSize);
  if (thisSize == dataSize) return false;
  dataSize -= thisSize;
  address += thisSize;
  
  return true;
}

uint8_t ApicTable::Iterator::GetType() {
  return *address;
}

void * ApicTable::Iterator::GetData() {
  return (void *)address;
}

size_t ApicTable::Iterator::GetSize() {
  return (size_t)address[1];
}

ApicTable::Iterator::Iterator(void * baseAddr, size_t size)
  : address((uint8_t *)baseAddr), dataSize(size) {
}

}

}
