#ifndef __ANARCH_API_DOMAIN_LIST_HPP__
#define __ANARCH_API_DOMAIN_LIST_HPP__

#include <ansa/module>

namespace anarch {

class DomainList : public ansa::Module {
public:
  static DomainList & GetGlobal();
  
  virtual int GetCount();
  virtual Domain & operator[](int idx);
};

}

#endif
