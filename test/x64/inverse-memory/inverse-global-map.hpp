#ifndef __ANARCH_TEST_INVERSE_GLOBAL_MAP_HPP__
#define __ANARCH_TEST_INVERSE_GLOBAL_MAP_HPP__

#include <anarch/api/global-map>
#include "log-list.hpp"

namespace anarch {

namespace dummy {

class InverseGlobalMap : public GlobalMap {
public:
  static InverseGlobalMap & GetGlobal();
  
  virtual void Set();

  virtual bool Read(PhysAddr * physOut, Attributes * attrOut,
                    PhysSize * sizeOut, VirtAddr addr);
  virtual bool Map(VirtAddr &, PhysAddr, Size, const Attributes &);
  virtual void MapAt(VirtAddr, PhysAddr, Size, const Attributes &);
  virtual void Unmap(VirtAddr, Size);
  virtual bool Reserve(VirtAddr &, Size);
  virtual void ReserveAt(VirtAddr, Size);
  
  PhysSize allowedPageSize = 0x1000;
  
  struct LogEntry {
    static const char ReadType = 'r';
    static const char MapType = 'm';
    static const char UnmapType = 'u';
    
    char type; // 'r' = read, 'm' = map, 'u' = unmap
    
    // some fields don't apply to every log type
    VirtAddr virtualAddr;
    PhysAddr physicalAddr;
    
    PhysSize pageSize;
    PhysSize pageCount;
    
    LogEntry * logNext = NULL;
  };
  
  LogList<LogEntry> log;
  
protected:
  ansa::DepList GetDependencies();
  virtual void Initialize();
};

}

}

#endif
