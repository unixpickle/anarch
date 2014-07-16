#include "tlb.hpp"
#include <anarch/critical>
#include <anarch/new>

namespace anarch {

namespace x64 {

namespace {

TLB globalTLB;

}

void TLB::Invlpg(VirtAddr addr) {
  // this shall be easy
  AssertCritical();
  __asm__ __volatile__("invlpg (%0)" : "r" (addr));
}

void TLB::InitGlobal() {
  new(&globalTLB) TLB();
}

TLB & TLB::GetGlobal() {
  return globalTLB;
}

void TLB::WillSetAddressSpace(MemoryMap &) {
  // TODO: something here
  AssertCritical();
}
  
ansa::DepList TLB::GetDependencies() {
  // TODO: something here
  return ansa::DepList();
}

void TLB::Initialize() {
  // TODO: this
}

}

}
