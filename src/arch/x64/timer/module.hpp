#ifndef __ANARCH_X64_TIMER_MODULE_HPP__
#define __ANARCH_X64_TIMER_MODULE_HPP__

#include <ansa/module>

namespace anarch {

namespace x64 {

class TimerModule : public ansa::Module {
public:
  static void InitGlobal();
  static TimerModule & GetGlobal();
  
protected:
  virtual ansa::DepList GetDependencies();
  virtual void Initialize();
  
private:
  static void CalibrateMethod();
};

}

}

#endif
