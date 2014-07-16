#include "page-table.hpp"
#include "scoped-scratch.hpp"
#include <anarch/api/panic>
#include <anarch/critical>

namespace anarch {

namespace x64 {

int PageTable::CalcDepth(PhysSize size) {
  switch (size) {
    case 0x1000:
      return 3;
    case 0x200000:
      return 2;
    case 0x40000000:
      return 1;
    default:
      break;
  }
  Panic("PageTable::CalcDepth() - unknown page size");
  return -1;
}

uint64_t PageTable::CalcMask(PhysSize, bool kernel, MemoryMap::Attributes) {
  return 1 | (pageSize == 0x1000 ? 0 : 0x80) | (kernel ? 0x100 : 0x4)
    | (attributes.executable ? 0 : (uint64_t)1 << 63)
    | (attirbutes.writable ? 2 : 0);
}

PageTable::PageTable(Allocator & a, Scratch & s, PhysAddr p)
  : allocator(&a), scratch(s), pml4(p) {
}

PageTable::PageTable(Allocator & a, Scratch & s) : allocator(&a), scratch(s) {
}

void PageTable::SetPML4(PhysAddr _pml4) {
  pml4 = _pml4;
}

PhysAddr PageTable::GetPML4() {
  return pml4;
}

void PageTable::SetAllocator(Allocator & a) {
  allocator = &a;
}

Allocator & PageTable::GetAllocator() {
  return *allocator;
}

int PageTable::Walk(VirtAddr addr, uint64_t & entry, size_t * size) {
  int indexes[4] = {
    (int)((addr >> 39) & 0x1ff),
    (int)((addr >> 30) & 0x1ff),
    (int)((addr >> 21) & 0x1ff),
    (int)((addr >> 12) & 0x1ff)
  };
  
  ScopedCritical critical;
  TypedScratch<uint64_t> table(scratch, pml4);
  int depth;
  for (depth = 0; depth < 3; depth++) {
    uint64_t nextPage = table[indexes[depth]];
    if (nextPage & 0x80 || !(nextPage & 1)) {
      break;
    }
    table.Reassign(nextPage & 0x7ffffffffffff000);
  }
  if (table[indexes[depth]] & 0x80 || depth == 3) {
    entry = table[indexes[depth]];
    if (size) *size = 0x1000L << (27 - 9 * depth);
    return depth;
  }
  if (size) *size = 0x1000L << (27 - 9 * depth);
  return -1;
}

bool PageTable::Set(VirtAddr addr, uint64_t entry, uint64_t parentMask,
                    int depth) {
  assert(theDepth >= 0 && theDepth < 4);
  assert(!(addr & (0x1000L << (27 - 9 * theDepth)) - 1));
  int indexes[4] = {
    (int)((addr >> 39) & 0x1ff),
    (int)((addr >> 30) & 0x1ff),
    (int)((addr >> 21) & 0x1ff),
    (int)((addr >> 12) & 0x1ff)
  };

  ScopedCritical critical;
  TypedScratch<uint64_t> table(scratch, pml4);
  for (int depth = 0; depth < theDepth; depth++) {
    uint64_t nextPage = table[indexes[depth]];
    if (!(nextPage & 1)) {
      // we have to temporarily leave the critical section, meaning afterwards
      // we will have to clear the scratch's CPU cache
      SetCritical(false);
      PhysAddr fresh;
      if (!allocator->Alloc(fresh, 0x1000, 0x1000)) {
        Panic("PageTable::Set() - allocation failed");
      }
      nextPage = (uint64_t)fresh;
      SetCritical(true);
      table.InvalidateCache();
    
      table[indexes[depth]] = nextPage | parentMask;
      table.Reassign(nextPage);
    
      // zero out the new page
      for (int i = 0; i < 0x200; i++) {
        table[i] = 0;
      }
    
      continue;
    } else if (nextPage & 0x80) {
      return false;
    } else {
      table[indexes[depth]] |= parentMask;
    }
    table.Reassign(nextPage & 0x7ffffffffffff000);
  }

  table[indexes[theDepth]] = entry;
  return true;
}

bool PageTable::Unset(VirtAddr addr) {
  assert(!(addr & 0xfff));
  int indexes[4] = {
    (int)((addr >> 39) & 0x1ff),
    (int)((addr >> 30) & 0x1ff),
    (int)((addr >> 21) & 0x1ff),
    (int)((addr >> 12) & 0x1ff)
  };

  PhysAddr tableAddresses[4] = {pml4, 0, 0, 0};

  ScopedCritical critical;
  TypedScratch<uint64_t> table(scratch, pml4);
  int maxDepth = 3;
  for (int depth = 0; depth < 3; depth++) {
    uint64_t nextPage = scratch[indexes[depth]];
    if (nextPage & 0x80) {
      maxDepth = depth;
      break;
    } else if (!(nextPage & 1)) {
      Panic("PageTable::Unset() - page not mapped!");
    }
    tableAddresses[depth + 1] = nextPage & 0x7ffffffffffff000;
    table.Reassign(tableAddresses[depth + 1]);
  }

  if (addr & (0x1000L << (27 - 9 * maxDepth)) - 1) {
    // the address is not properly aligned to the page we found
    return false;
  }

  // zero out the page and delete any parent table indexes
  table[indexes[maxDepth]] = 0;

  for (int i = maxDepth; i > 0; i--) {
    bool allGone = true;
    for (int j = 0; j < 0x200; j++) {
      if (table[j]) {
        allGone = false;
        break;
      }
    }
    if (!allGone) break;
  
    SetCritical(false);
    allocator->Free(tableAddresses[i]);
    SetCritical(true);
    // no need to run table.InvalidateCache() because we are reassigning it
    
    table.Reassign(tableAddresses[i - 1]);
    table[indexes[i - 1]] = 0;
  }

  return true;
}

}

}