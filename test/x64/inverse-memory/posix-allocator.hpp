#ifndef __ANARCH_TEST_POSIX_ALLOCATOR_HPP__
#define __ANARCH_TEST_POSIX_ALLOCATOR_HPP__

#include "log-list.hpp"
#include <anarch/api/allocator>
#include <ansa/atomic>

namespace anarch {

namespace dummy {

class PosixAllocator : public Allocator {
public:
  virtual bool Alloc(PhysAddr & result, PhysSize size, PhysSize align);
  virtual void Free(PhysAddr address);
  virtual PhysSize Used();
  virtual PhysSize Available();
  virtual PhysSize Total();
  
  long GetAllocationCount();
  
  struct LogEntry {
    bool allocation;
    PhysAddr address;
    
    PhysSize size;
    PhysSize align;
    
    LogEntry * logNext = NULL;
    
    LogEntry(bool a, PhysAddr b, PhysSize c, PhysSize d)
      : allocation(a), address(b), size(c), align(d) {}
  };
  
  LogList<LogEntry> log;
  
private:
  ansa::Atomic<long> allocationCount;
};

}

}

#endif
