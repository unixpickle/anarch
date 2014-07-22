#include "module.hpp"
#include "../interrupts/pic.hpp"
#include "../interrupts/irt.hpp"
#include <anarch/stream>
#include <anarch/new>

namespace anarch {

namespace x64 {

namespace {

CriticalModule gCriticalModule;

}

void CriticalModule::InitGlobal() {
  new(&gCriticalModule) CriticalModule();
}

CriticalModule & CriticalModule::GetGlobal() {
  return gCriticalModule;
}

ansa::DepList CriticalModule::GetDependencies() {
  return ansa::DepList(&Irt::GetGlobal(), &Pic::GetGlobal(),
                       &StreamModule::GetGlobal());
}

void CriticalModule::Initialize() {
  cout << "Initializing criticality subsystem...";
  
  Irt::GetGlobal().ConfigurePicEoi();
  __asm__ __volatile__("sti\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
                       "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\ncli");
  Irt::GetGlobal().Configure();
  
  __asm__ __volatile__("sti");
  cout << " [OK]" << endl;
}

}

}
