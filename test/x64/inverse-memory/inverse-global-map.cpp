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
  return 1;
}

PhysSize GlobalMap::GetPageSize(int) {
  return 0x1000;
}

PhysSize GlobalMap::GetPageSizeAlign(int) {
  return 0x1000;
}

bool GlobalMap::SupportsReadAddress() {
  return true;
}

bool GlobalMap::SupportsReadAttributes() {
  return false;
}

bool GlobalMap::SupportsReadSize() {
  return true;
}

bool GlobalMap::SupportsReserveAt() {
  return false;
}

bool GlobalMap::SupportsReserve() {
  return false;
}

bool GlobalMap::SupportsMapAt() {
  return false;
}

bool GlobalMap::SupportsNX() {
  return false;
}

bool GlobalMap::SupportsRO() {
  return false;
}

bool GlobalMap::SupportsUncached() {
  return false;
}

namespace dummy {

void InverseGlobalMap::Set() {
}

bool InverseGlobalMap::Read(PhysAddr * physOut, Attributes *,
                             PhysSize * sizeOut, VirtAddr addr) {
  if (physOut) *physOut = InvertPhys(addr);
  if (sizeOut) *sizeOut = 0x1000;
  return true;
}

bool InverseGlobalMap::Map(VirtAddr & res, PhysAddr phys,
                            Size, const Attributes &) {
  res = InvertPhys(phys);
  return true;
}

void InverseGlobalMap::MapAt(VirtAddr, PhysAddr, Size, const Attributes &) {
  Panic("InverseGlobalMap::MapAt() - not supported");
}

void InverseGlobalMap::Unmap(VirtAddr, Size) {
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
