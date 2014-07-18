#include "scratch.hpp"
#include "tlb.hpp"
#include <anarch/api/panic>
#include <anarch/critical>
#include <ansa/cstring>

namespace anarch {

namespace x64 {

Scratch::Scratch(uint64_t * ptStart) : pageTables(ptStart), bitmap(false) {
}

void Scratch::CreateMappings(uint64_t * pdpt, Allocator & allocator) {
  PhysAddr scratchPDT;
  if (!allocator.Alloc(scratchPDT, 0x1000, 0x1000)) {
    Panic("Scratch::CreateMappings() - failed to allocate PDT");
  }
  ansa::Bzero((void *)scratchPDT, 0x1000);

  // put the scratch PDT into the kernel PDPT
  pdpt[0x1ff] = scratchPDT | 3;

  // setup the page tables and put them in the scratch PDT
  for (int i = 0; i < PTCount; i++) {
    PhysAddr scratchPT = (PhysAddr)(&pageTables[0x200 * i]);
    ((uint64_t *)scratchPDT)[i] = scratchPT | 3;
  }
}

bool Scratch::Alloc(VirtAddr & addr, PhysAddr page) {
  AssertCritical();
  ScopedLock scope(lock);
  
  for (size_t i = 0; i < PTCount * 0x200; i++) {
    if (!bitmap[i]) {
      bitmap[i] = true;
      pageTables[i] = page | 3;
      addr = CellVirtual(i);
      TLB::Invlpg(addr);
      return true;
    }
  }
  return false;
}

void Scratch::Reassign(VirtAddr addr, PhysAddr newAddr) {
  AssertCritical();
  size_t idx = (addr - CellVirtual(0)) / 0x1000;
  pageTables[idx] = newAddr | 3;
  TLB::Invlpg(addr);
}

void Scratch::Free(VirtAddr addr) {
  AssertCritical();
  ScopedLock scope(lock);
  size_t idx = (addr - CellVirtual(0)) / 0x1000;
  bitmap[idx] = false;
}

VirtAddr Scratch::CellVirtual(size_t cellIdx) {
  return StartAddr + cellIdx * 0x1000;
}

}

}
