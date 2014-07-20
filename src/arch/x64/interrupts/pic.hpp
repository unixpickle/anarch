#ifndef __ANARCH_X64_PIC_HPP__
#define __ANARCH_X64_PIC_HPP__

#include <anarch/stdint>
#include <ansa/module>

namespace anarch {

namespace x64 {

class Pic : public ansa::Module {
public:
  static void InitGlobal();
  static Pic & GetGlobal();
  
  void Remap(uint8_t masterBase, uint8_t slaveBase, uint8_t masterMask,
             uint8_t slaveMask);
  
protected:
  virtual ansa::DepList GetDependencies();
  virtual void Initialize(); // disables PIC
};

}

}

#endif
