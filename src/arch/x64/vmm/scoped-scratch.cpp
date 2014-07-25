#include "scoped-scratch.hpp"
#include "tlb.hpp"
#include <anarch/api/panic>
#include <anarch/critical>

namespace anarch {

namespace x64 {

ScopedScratch::ScopedScratch(Scratch & s, PhysAddr phys) : scratch(s) {
  AssertCritical();
  if (!scratch.Alloc(addr, phys)) {
    Panic("ScopedScrath::ScopedScrath() - alloc failed");
  }
}

ScopedScratch::~ScopedScratch() {
  scratch.Free(addr);
}

void * ScopedScratch::GetPointer() {
  return (void *)addr;
}

VirtAddr ScopedScratch::GetVirtAddr() {
  return addr;
}

void ScopedScratch::Reassign(PhysAddr phys) {
  AssertCritical();
  scratch.Reassign(addr, phys);
}

void ScopedScratch::InvalidateCache() {
  AssertCritical();
  Tlb::Invlpg(addr);
}

}

}
