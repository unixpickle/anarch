#include "delegate.hpp"
#include <anarch/stddef>

namespace anarch {

namespace {

Delegate * gDelegate = NULL;

}

Delegate * Delegate::GetGlobalDelegate() {
  return gDelegate;
}

void Delegate::SetGlobalDelegate(Delegate * d) {
  gDelegate = d;
}

}