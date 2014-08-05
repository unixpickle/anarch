#ifndef __ANARCH_X64_PANIC_HPP__
#define __ANARCH_X64_PANIC_HPP__

#include <ansa/macros>
#include <ansa/module>
#include <ansa/atomic>

namespace anarch {

namespace x64 {

class PanicModule : public ansa::Module {
public:
  static void InitGlobal();
  static PanicModule & GetGlobal();
  
  void Panic(const char * msg) ANSA_NORETURN;
  
protected:
  virtual void Initialize();
  virtual ansa::DepList GetDependencies();
  
private:
  ansa::Atomic<int> panicCount;
  
  void DistributeError();
  static void WriteError(const char * msg);
  static void HaltCpu();
};

}

}

#endif
