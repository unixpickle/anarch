#include "startup-code.hpp"
#include "../segments/gdt.hpp"
#include "../vmm/global/global-malloc.hpp"
#include "../vmm/global/global-map.hpp"
#include <anarch/api/panic>
#include <ansa/cstring>

extern "C" {

extern void anarch_proc_entry() __asm__("anarch_proc_entry");
extern void anarch_proc_entry_end() __asm__("anarch_proc_entry_end");

}

namespace anarch {

namespace x64 {

StartupCode::StartupCode(void (*_cb)()) : callbackFunc((uint64_t)_cb) {
  VirtualAllocator & allocator = GlobalMalloc::GetGlobal().GetAllocator();
  if (!allocator.Alloc(dataBackup, 0x3000)) {
    Panic("StartupCode::StartupCode() - failed to allocate backup");
  }
  ansa::Memcpy(dataBackup, (void *)PageTableBase, 0x3000);
  
  // set the PDPT in the dummy PML4
  PhysAddr pdpt = GlobalMap::GetGlobal().GetPDPT();
  *((uint64_t *)PageTableBase) = pdpt | 3;
  
  // copy the GDT pointer
  Gdt::Pointer ptr = Gdt::GetGlobal().GetPointer();
  ansa::Memcpy((void *)(PageTableBase + 0x1000), &ptr, sizeof(ptr));
  
  // copy the bootstrap code
  void * bootstrapCode = (void *)&anarch_proc_entry;
  void * bootstrapEnd = (void *)&anarch_proc_entry_end;
  uint64_t codeLen = (uint64_t)bootstrapEnd - (uint64_t)bootstrapCode;
  assert(codeLen < 0x1000);
  ansa::Memcpy((void *)CodeBase, bootstrapCode, codeLen);
}

StartupCode::~StartupCode() {
  ansa::Memcpy((void *)PageTableBase, dataBackup, 0x3000);
  
  VirtualAllocator & allocator = GlobalMalloc::GetGlobal().GetAllocator();
  allocator.Free(dataBackup);
}

void StartupCode::ResetStartupStack() {
  uint64_t * bootstrapStack = (uint64_t *)CodeBase - 2;
  PhysAddr pml4 = GlobalMap::GetGlobal().GetPageTable().GetPML4();
  bootstrapStack[0] = (uint64_t)pml4;
  bootstrapStack[1] = callbackFunc;
}

}

}
