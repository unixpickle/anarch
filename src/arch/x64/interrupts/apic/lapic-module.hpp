#ifndef __ANARCH_X64_LAPIC_MODULE_HPP__
#define __ANARCH_X64_LAPIC_MODULE_HPP__

#include "x2apic.hpp"
#include "xapic.hpp"
#include <ansa/module>
#include <ansa/macros>

namespace anarch {

namespace x64 {

class LapicModule : public ansa::Module {
public:
  static void InitGlobal();
  static LapicModule & GetGlobal();
  
  virtual Lapic & GetLapic(); // @critical
  
protected:
  virtual ansa::DepList GetDependencies();
  virtual void Initialize();
  
private:
  char xapicData[sizeof(XApic)] ANSA_ALIGNED(8);
  char x2apicData[sizeof(X2Apic)] ANSA_ALIGNED(8);
};

}

}

#endif
