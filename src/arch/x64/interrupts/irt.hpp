#ifndef __ANARCH_X64_IRT_HPP__
#define __ANARCH_X64_IRT_HPP__

#include <anarch/lock>
#include <ansa/atomic>
#include <ansa/module>

namespace anarch {

namespace x64 {

class Irt : public ansa::Module {
public:
  static void InitGlobal();
  static Irt & GetGlobal();
  
  void * Get(uint8_t idx); // @ambicritical
  void Set(uint8_t idx, void * r); // @ambicritical
  void Unset(uint8_t idx); // @ambicritical
  
  void ConfigurePicEoi();
  void Configure();
  
protected:
  virtual ansa::DepList GetDependencies();
  virtual void Initialize();
  
private:
  ansa::Atomic<uint64_t> routines[0x100];
};

}

}

#endif
