#include "gdt.hpp"
#include "../vmm/global/global-malloc.hpp"
#include <anarch/api/panic>
#include <anarch/critical>
#include <anarch/new>
#include <ansa/cstring>

namespace anarch {

namespace x64 {

namespace {

Gdt gGdt;

}

Gdt::Pointer Gdt::Pointer::GetCurrent() {
  AssertCritical();
  Pointer result;
  __asm__ __volatile__("sgdt (%0)" : : "r" (&result));
  return result;
}

void Gdt::InitGlobal() {
  new(&gGdt) Gdt();
}

Gdt & Gdt::GetGlobal() {
  return gGdt;
}

void Gdt::Set() {
  AssertCritical();
  Pointer ptr = GetPointer();
  __asm__ __volatile__("lgdt (%0)" : : "r" (&ptr));
}

Gdt::Pointer Gdt::GetPointer() {
  Pointer ptr = {0xffff, (uint64_t)buffer};
  return ptr;
}

ansa::DepList Gdt::GetDependencies() {
  return ansa::DepList(&GlobalMalloc::GetGlobal());
}

void Gdt::Initialize() {
  if (!GlobalMalloc::GetGlobal().GetAllocator().Alloc(buffer, 0x10000)) {
    Panic("Gdt::Initialize() - failed to allocate buffer");
  }
  ansa::Bzero(buffer, 0x10000);
  
  // get the current GDT pointer info
  Pointer current = Pointer::GetCurrent();
  amountUsed = (size_t)current.limit + 1;
  ansa::Memcpy(buffer, (void *)current.start, amountUsed);
}

}

}
