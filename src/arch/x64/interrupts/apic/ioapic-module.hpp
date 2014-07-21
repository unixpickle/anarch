#ifndef __ANACRH_X64_IOAPIC_MODULE_HPP__
#define __ANACRH_X64_IOAPIC_MODULE_HPP__

#include "ioapic.hpp"
#include <ansa/module>

namespace anarch {

namespace x64 {

class IOApicModule : public ansa::Module {
public:
  static void InitGlobal();
  static IOApicModule & GetGlobal();
  
  IOApic & GetBaseIOApic();
  
protected:
  virtual ansa::DepList GetDependencies();
  virtual void Initialize();
  
private:
  IOApic * baseIOApic;
};

}

}

#endif
