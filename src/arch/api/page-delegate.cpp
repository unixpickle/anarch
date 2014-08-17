#include "page-delegate.hpp"
#include <anarch/stddef>

namespace anarch {

namespace {

PageDelegate gDelegate = NULL;

}

void SetGlobalPageDelegate(PageDelegate obj) {
  gDelegate = obj;
}

PageDelegate GetGlobalPageDelegate() {
  return gDelegate;
}

}
