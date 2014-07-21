#include "inverse-scoped-scratch.hpp"
#include "inverter.hpp"
#include <ansa/atomic>
#include "../../../src/arch/x64/vmm/scoped-scratch.hpp"

namespace anarch {

namespace x64 {

ansa::Atomic<long> scopeUsage;

long GetInverseScopedScratchUsage() {
  return scopeUsage;
}

ScopedScratch::ScopedScratch(Scratch & s, PhysAddr a) : scratch(s) {
  addr = InvertPhys(a);
  ++scopeUsage;
}

ScopedScratch::~ScopedScratch() {
  --scopeUsage;
}

void * ScopedScratch::GetPointer() {
  return (void *)addr;
}

VirtAddr ScopedScratch::GetVirtAddr() {
  return addr;
}

void ScopedScratch::Reassign(PhysAddr a) {
  addr = InvertPhys(a);
}

void ScopedScratch::InvalidateCache() {
}

}

}
