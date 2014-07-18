#include "identity-scoped-scratch.hpp"
#include <ansa/atomic>
#include "../../../src/arch/x64/vmm/scoped-scratch.hpp"

namespace anarch {

namespace x64 {

ansa::Atomic<long> scopeUsage;

long GetIdentityScopedScratchUsage() {
  return scopeUsage;
}

ScopedScratch::ScopedScratch(Scratch & s, PhysAddr a) : scratch(s) {
  addr = a;
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
  addr = a;
}

void ScopedScratch::InvalidateCache() {
}

}

}
