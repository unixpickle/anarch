#include "acpi-module.hpp"
#include "acpi-pointer.hpp"
#include "../vmm/global/global-malloc.hpp"
#include <anarch/assert>

namespace anarch {

namespace x64 {

namespace {

AcpiModule gAcpiModule;

}

void AcpiModule::InitGlobal() {
  new(&gAcpiModule) AcpiModule();
}

AcpiModule & AcpiModule::GetGlobal() {
  return gAcpiModule;
}

AcpiRoot & AcpiModule::GetRoot() {
  return *root;
}

ansa::DepList AcpiModule::GetDependencies() {
  return ansa::DepList(&GlobalMalloc::GetGlobal());
}

void AcpiModule::Initialize() {
  AcpiPointer * pointer = AcpiPointer::Find();
  assert(pointer != NULL);
  
  root = pointer->GenerateRoot();
}

}

}
