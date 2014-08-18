#include "domain.hpp"
#include "domain-list.hpp"
#include <anarch/api/panic>

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
  return false;
}

void Domain::Free(void * ptr) {
  if (GetVirtualAllocator().Owns(ptr)) {
    GetVirtualAllocator().Free(ptr);
    return;
  }
  
  int siblingCount = DomainList::GetGlobal().GetCount() - 1;
  for (int i = 0; i < siblingCount; i++) {
    VirtualAllocator & a = GetSibling(i).GetVirtualAllocator();
    if (a.Owns(ptr)) {
      a.Free(ptr);
      return;
    }
  }
  
  Panic("Domain::Free() - no Domain owns the specified memory");
}

bool Domain::AllocPhys(PhysAddr & addrOut, PhysSize size, PhysSize align) {
  if (GetAllocator().Alloc(addrOut, size, align)) {
    return true;
  }
  
  int siblingCount = DomainList::GetGlobal().GetCount() - 1;
  for (int i = 0; i < siblingCount; ++i) {
    Allocator & a = GetSibling(i).GetAllocator();
    if (a.Alloc(addrOut, size, align)) {
      return true;
    }
  }
  
  return false;
}

void Domain::FreePhys(PhysAddr addr) {
  if (GetAllocator().Owns(addr)) {
    GetAllocator().Free(addr);
    return;
  }
  
  int siblingCount = DomainList::GetGlobal().GetCount() - 1;
  for (int i = 0; i < siblingCount; ++i) {
    Allocator & a = GetSibling(i).GetAllocator();
    if (a.Owns(addr)) {
      a.Free(addr);
      return;
    }
  }
  
  Panic("Domain::FreePhys() - no Domain owns the physical address");
}

}
