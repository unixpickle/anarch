#ifndef __ANARCH_X64_IDT_HPP__
#define __ANARCH_X64_IDT_HPP__

#include "idt-types.hpp"
#include <ansa/module>

namespace anarch {

namespace x64 {

class Idt : public ansa::Module {
public:
  static void InitGlobal();
  static Idt & GetGlobal();
  
  void SetEntry(int idx, void * fn, uint8_t flags);
  void SetIst(int idx, uint8_t ist);
  
  void Set();
  
protected:
  virtual ansa::DepList GetDependencies();
  
private:
  IdtEntry entries[0x100] ANSA_ALIGNED(8);
  IdtHandler handlers[0x100];
  
  IdtPointer idtPtr ANSA_ALIGNED(8);
};

}

}

#endif
