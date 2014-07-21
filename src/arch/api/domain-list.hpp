#ifndef __ANARCH_API_DOMAIN_LIST_HPP__
#define __ANARCH_API_DOMAIN_LIST_HPP__

#include "domain.hpp"
#include <ansa/module>

namespace anarch {

class DomainList : public ansa::Module {
public:
  static DomainList & GetGlobal();
  
  virtual int GetCount() = 0;
  virtual Domain & operator[](int idx) = 0;
};

}

#endif
