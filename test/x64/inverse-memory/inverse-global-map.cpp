#include "inverse-global-map.hpp"
#include "inverter.hpp"
#include <anarch/api/panic>

namespace anarch {

namespace {

dummy::InverseGlobalMap gObject;

}

GlobalMap & GlobalMap::GetGlobal() {
  return gObject;
}

int GlobalMap::GetPageSizeCount() {
  return 2;
}

PhysSize GlobalMap::GetPageSize(int i) {
  if (i == 0) return 0x1000;
  return 0x200000;
}

PhysSize GlobalMap::GetPageSizeAlign(int i) {
  return GetPageSize(i);
}

GlobalMap::Capabilities GlobalMap::GetCapabilities() {
  Capabilities caps;
  caps.placementReserve = false;
  caps.placementMap = false;
  caps.executableFlag = false;
  caps.writableFlag = false;
  caps.cachableFlag = false;
  return caps;
}

namespace dummy {

InverseGlobalMap & InverseGlobalMap::GetGlobal() {
  return gObject;
}

void InverseGlobalMap::Set() {
}

bool InverseGlobalMap::Read(PhysAddr * physOut, Attributes *,
                            PhysSize * sizeOut, VirtAddr addr) {
  if (physOut) *physOut = InvertPhys(addr);
  if (sizeOut) *sizeOut = allowedPageSize;
  
  LogEntry * entry = new LogEntry();
  entry->type = LogEntry::ReadType;
  entry->virtualAddr = addr;
  entry->physicalAddr = InvertPhys(addr);
  entry->pageSize = allowedPageSize;
  log.Push(entry);
  
  return true;
}

bool InverseGlobalMap::Map(VirtAddr & res, PhysAddr phys,
                            Size theSize, const Attributes &) {
  res = InvertPhys(phys);
  
  LogEntry * entry = new LogEntry();
  entry->type = LogEntry::MapType;
  entry->virtualAddr = res;
  entry->physicalAddr = phys;
  entry->pageSize = theSize.pageSize;
  entry->pageCount = theSize.pageCount;
  log.Push(entry);
  
  if (theSize.pageSize != allowedPageSize) {
    return false;
  }
  
  return true;
}

void InverseGlobalMap::MapAt(VirtAddr, PhysAddr, Size, const Attributes &) {
  Panic("InverseGlobalMap::MapAt() - not supported");
}

void InverseGlobalMap::Unmap(VirtAddr virt, Size size) {
  LogEntry * entry = new LogEntry();
  entry->type = LogEntry::UnmapType;
  entry->virtualAddr = virt;
  entry->physicalAddr = InvertPhys(virt);
  entry->pageSize = size.pageSize;
  entry->pageCount = size.pageCount;
  log.Push(entry);
}

bool InverseGlobalMap::Reserve(VirtAddr &, Size) {
  Panic("InverseGlobalMap::Reserve() - not supported");
  return false;
}

void InverseGlobalMap::ReserveAt(VirtAddr, Size) {
  Panic("InverseGlobalMap::ReserveAt() - not supported");
}

ansa::DepList InverseGlobalMap::GetDependencies() {
  return ansa::DepList();
}

void InverseGlobalMap::Initialize() {
}

}

}
