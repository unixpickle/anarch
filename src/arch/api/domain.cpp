#include "domain.hpp"

namespace anarch {

bool Domain::Alloc(void *& ptr, size_t size) {
  if (GetVirtualAllocator().Alloc(ptr, size)) {
    return true;
  }
  
  int siblingCount = DomainList::GetGlobal().GetCount() - 1;
  for (int i = 0; i < siblingCount; i++) {
    if (GetSibling(i).GetVirtualAllocator().Alloc(ptr, size)) {
      return true;
    }
  }
}

void Domain::Free(void * ptr) {
  if (GetVirtualAllocaton().Owns(ptr)) {
    GetVirtualAllocator().Free(ptr);
    return;
  }
  
  int siblingCount = DomainList::GetGlobal().GetCount() - 1;
  for (int i = 0; i < siblingCount; i++) {
    VirtualAllocator & a = GetSibling(i).GetVirtualAllocaton();
    if (a.Owns(ptr)) {
      a.Free(ptr);
      return;
    }
  }
}

}
