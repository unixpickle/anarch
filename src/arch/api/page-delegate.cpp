#include "page-delegate.hpp"
#include <anarch/stddef>

namespace anarch {

namespace {

PageDelegate * gDelegate = NULL;

}

PageDelegate * PageDelegate::GetGlobal() {
  return gDelegate;
}

void PageDelegate::SetGlobal(PageDelegate * d) {
  gDelegate = d;
}

}
