#ifndef __ANARCH_X64_CRITICAL_MODULE_HPP__
#define __ANARCH_X64_CRITICAL_MODULE_HPP__

#include <ansa/module>

namespace anarch {

namespace x64 {

class CriticalModule : public ansa::Module {
public:
  static void InitGlobal();
  static CriticalModule & GetGlobal();
  
protected:
  virtual ansa::DepList GetDependencies();
  virtual void Initialize();
};

}

}

#endif
