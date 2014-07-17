#ifndef __ANARCH_TEST_IDENTITY_SCRATCH_HPP__
#define __ANARCH_TEST_IDENTITY_SCRATCH_HPP__

#include "../../../src/arch/x64/vmm/scratch.hpp"
#include <anarch/api/panic>

namespace anarch {

namespace x64 {

Scratch::Scratch(uint64_t *) {  
}

void Scratch::CreateMappings(uint64_t *, Allocator &) {
}

bool Scratch::Alloc(VirtAddr & addr, PhysAddr page) {
  addr = page;
  return true;
}

void Scratch::Reassign(VirtAddr, PhysAddr) {
  Panic("identity scratch does not support Reassign()");
}

void Scratch::Free(VirtAddr) {
  
}

}

}

#endif
