#ifndef __ARCH_X64_DOMAIN_LIST_HPP__
#define __ARCH_X64_DOMAIN_LIST_HPP__

#include "domain.hpp"
#include <anarch/api/domain-list>

namespace anarch {

namespace x64 {

/**
 * Since the x86-64 anarch APIs do not yet support NUMA, the DomainList simply
 * initializes every CPU on the system and throws it into one Domain.
 */
class DomainList : public anarch::DomainList {
public:
  static void InitGlobal();
  static DomainList & GetGlobal();
  
  virtual int GetCount();
  virtual Domain & operator[](int idx);
  
protected:
  virtual ansa::DepList GetDependencies();
  virtual void Initialize();
  
private:
  Domain * mainDomain;
};

}

}

#endif
