#include "irt.hpp"
#include "raw-isr.hpp"
#include "idt.hpp"
#include <anarch/stream>
#include <anarch/new>

namespace anarch {

namespace x64 {

namespace {

Irt gIrt;

}

void Irt::InitGlobal() {
  new(&gIrt) Irt();
}

Irt & Irt::GetGlobal() {
  return gIrt;
}

void * Irt::Get(uint8_t idx) {
  return (void *)((uint64_t)routines[idx]);
}

void Irt::Set(uint8_t idx, void * r) {
  routines[idx] = (uint64_t)r;
}

void Irt::Unset(uint8_t idx) {
  routines[idx] = 0;
}

void Irt::ConfigurePicEoi() {
  Idt & globalIdt = Idt::GetGlobal();
  for (int i = 0; i < 0x100; i++) {
    globalIdt.SetEntry(i, (void *)RawPicEoiIsr, 0x8e);
  }
}

void Irt::Configure() {
  Idt & globalIdt = Idt::GetGlobal();
  for (int i = 0; i < 0x100; i++) {
    bool hasCode = (i == 0x8 || i == 0x11 || (i >= 0xa && i <= 0xe));
    if (hasCode) {
      globalIdt.SetEntry(i, (void *)RawCodedIsr, 0x8e);
    } else {
      globalIdt.SetEntry(i, (void *)RawNonCodedIsr, 0x8e);
    }
  }
}

ansa::DepList Irt::GetDependencies() {
  return ansa::DepList(&Idt::GetGlobal(), &StreamModule::GetGlobal());
}

void Irt::Initialize() {
  cout << "Initializing Interrupt Routine Table...";
  Idt::GetGlobal().Set();
  cout << " [OK]" << endl;
}

}

}
