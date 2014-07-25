#ifndef __ANARCH_X64_DOMAIN_HPP__
#define __ANARCH_X64_DOMAIN_HPP__

#include "../smp/cpu.hpp"
#include <anarch/api/domain>

namespace anarch {

namespace x64 {

class Domain : public anarch::Domain {
public:
  Domain(int capacity);
  ~Domain();

  Cpu & GetCpu(int idx); // @ambicritical
  int InitNewCpu(); // @noncritical

  // anarch::Domain
  virtual anarch::Domain & GetSibling(int);
  virtual int GetThreadCount();
  virtual Thread & GetThread(int idx);
  virtual Allocator & GetAllocator();
  virtual VirtualAllocator & GetVirtualAllocator();

private:
  Allocator & allocator;
  VirtualAllocator & virtualAllocator;
  
  char * cpuData;
  int capacity;
  int count = 0;
};

}

}

#endif
