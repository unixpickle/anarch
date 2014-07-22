#include "global-malloc.hpp"
#include "global-map.hpp"
#include <anarch/stream>
#include <anarch/new>

namespace anarch {

namespace x64 {

namespace {

GlobalMalloc gMalloc;

}

void GlobalMalloc::InitGlobal() {
  new(&gMalloc) GlobalMalloc();
}

GlobalMalloc & GlobalMalloc::GetGlobal() {
  return gMalloc;
}

VirtualAllocator & GlobalMalloc::GetAllocator() {
  return *(Malloc *)mallocBuf;
}

ansa::DepList GlobalMalloc::GetDependencies() {
  return ansa::DepList(&GlobalMap::GetGlobal(), &StreamModule::GetGlobal());
}

void GlobalMalloc::Initialize() {
  cout << "Initializing GlobalMalloc...";
  Malloc * mallocPtr = (Malloc *)mallocBuf;
  new(mallocPtr) Malloc(0x200000, GlobalMap::GetGlobal().GetPageAllocator());
  cout << " [OK]" << endl;
}

}

}
