#include "domain.hpp"
#include "domain-list.hpp"
#include "../vmm/global/global-map.hpp"
#include "../vmm/global/global-malloc.hpp"
#include <anarch/api/panic>

namespace anarch {

Domain & Domain::GetCurrent() {
  return DomainList::GetGlobal()[0];
}

namespace x64 {

Domain::Domain(int _capacity)
  : allocator(GlobalMap::GetGlobal().GetPageAllocator()),
    virtualAllocator(GlobalMalloc::GetGlobal().GetAllocator()),
    capacity(_capacity) {
  void * data;
  if (!GetVirtualAllocator().Alloc(data, sizeof(Cpu) * capacity)) {
    Panic("Domain::Domain() - failed to allocate CPUs");
  }
  cpuData = (char *)data;
}

Domain::~Domain() {
  GetVirtualAllocator().Free((void *)cpuData);
}

Cpu & Domain::GetCpu(int idx) {
  Cpu * cpuPtr = (Cpu *)cpuData;
  return cpuPtr[idx];
}

int Domain::InitNewCpu() {
  assert(count < capacity);
  Cpu * cpuPtr = (Cpu *)cpuData + count;
  new(cpuPtr) Cpu();
  return count++;
}

anarch::Domain & Domain::GetSibling(int) {
  Panic("Domain::GetSibling() - not supported by x64");
  return *((anarch::Domain *)NULL);
}

int Domain::GetThreadCount() {
  return count;
}

Thread & Domain::GetThread(int idx) {
  return GetCpu(idx);
}

Allocator & Domain::GetAllocator() {
  return allocator;
}

VirtualAllocator & Domain::GetVirtualAllocator() {
  return virtualAllocator;
}

}

}
