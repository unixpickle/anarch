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

uint16_t Gdt::PushShortDescriptor(const ShortDescriptor & desc) {
  assert(amountUsed + 8 <= 0x10000);
  uint16_t sel = (uint16_t)amountUsed;
  ansa::Memcpy((void *)((uint64_t)buffer + amountUsed), (void *)&desc, 8);
  amountUsed += 8;
  return sel;
}

uint16_t Gdt::PushTssDescriptor(const TssDescriptor & desc) {
  assert(amountUsed + 0x10 <= 0x10000);
  uint16_t sel = (uint16_t)amountUsed;
  ansa::Memcpy((void *)((uint64_t)buffer + amountUsed), (void *)&desc, 0x10);
  amountUsed += 0x10;
  return sel;
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
  
  amountUsed = 8;
  
  // code, data, user data, user code
  PushShortDescriptor(ShortDescriptor(true, true)); // kernel code
  PushShortDescriptor(ShortDescriptor(true, false)); // kernel data
  PushShortDescriptor(ShortDescriptor(false, false)); // user data
  PushShortDescriptor(ShortDescriptor(false, true)); // user code
  
  ScopedCritical critical;
  Set();
}

}

}
