#include "identity-global-map.hpp"
#include <anarch/api/panic>

namespace anarch {

namespace {

dummy::IdentityGlobalMap gObject;

}

GlobalMap & GlobalMap::GetGlobal() {
  return gObject;
}

int GlobalMap::GetPageSizeCount() {
  return 1;
}

PhysSize GlobalMap::GetPageSize(int) {
  return 1;
}

PhysSize GlobalMap::GetPageSizeAlign(int) {
  return 1;
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

void IdentityGlobalMap::Set() {
}

bool IdentityGlobalMap::Map(VirtAddr & res, PhysAddr phys,
                            Size, const Attributes &) {
  res = phys;
  return true;
}

void IdentityGlobalMap::MapAt(VirtAddr, PhysAddr, Size, const Attributes &) {
  Panic("IdentityGlobalMap::MapAt() - not supported");
}

void IdentityGlobalMap::Unmap(VirtAddr, Size) {
}

bool IdentityGlobalMap::Reserve(VirtAddr &, Size) {
  Panic("IdentityGlobalMap::Reserve() - not supported");
  return false;
}

void IdentityGlobalMap::ReserveAt(VirtAddr, Size) {
  Panic("IdentityGlobalMap::ReserveAt() - not supported");
}

ansa::DepList IdentityGlobalMap::GetDependencies() {
  return ansa::DepList();
}

void IdentityGlobalMap::Initialize() {
}

}

}
