#include "../../../src/arch/x64/vmm/scoped-scratch.hpp"

namespace anarch {

namespace x64 {

ScopedScratch::ScopedScratch(Scratch & s, PhysAddr a) : scratch(s) {
  addr = a;
}

ScopedScratch::~ScopedScratch() {
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
