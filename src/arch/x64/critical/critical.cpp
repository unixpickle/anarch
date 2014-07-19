#include "critical.hpp"

namespace anarch {

namespace {

bool ignoreCriticality = true;

}

bool IgnoreCriticality() {
  return ignoreCriticality;
}

bool GetCritical() {
  if (IgnoreCriticality()) return true;

  unsigned long value;
  __asm__("pushfq\n"
          "pop %0" : "=r" (value));
  return (value & (1 << 9)) == 0;
}

void SetCritical(bool flag) {
  if (IgnoreCriticality()) return;

  if (flag) {
    __asm__("cli");
  } else {
    __asm__("sti");
  }
}

namespace x64 {

void SetIgnoreCriticality(bool flag) {
  ignoreCriticality = flag;
}

}

}
