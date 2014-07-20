#include "idt.hpp"
#include <anarch/new>
#include <anarch/assert>

namespace anarch {

namespace x64 {

namespace {

Idt gIdt;

}

void Idt::InitGlobal() {
  new(&gIdt) Idt();
}

Idt & Idt::GetGlobal() {
  return gIdt;
}

void Idt::SetEntry(int idx, void * fn, uint8_t flags) {
  handlers[idx].function = (uint64_t)fn;
  handlers[idx].argument = (uint64_t)idx;
  entries[idx].SetOffset((uint64_t)&handlers[idx]);
  entries[idx].flags = flags;
}

void Idt::SetIst(int idx, uint8_t ist) {
  entries[idx].ist = ist;
}

void Idt::Set() {
  idtPtr.limit = 0xfff; // TODO: see what the real maximum should be
  idtPtr.virtualAddress = (uint64_t)entries;
  assert(!((uint64_t)&idtPtr & 0x7));
  assert(!((uint64_t)entries & 0x7));
  __asm__ __volatile__("lidt (%0)" : : "r" (&idtPtr));
}

ansa::DepList Idt::GetDependencies() {
  return ansa::DepList();
}

}

}

