#ifndef __ANARCH_X64_GDT_HPP__
#define __ANARCH_X64_GDT_HPP__

#include <anarch/stdint>
#include <anarch/stddef>
#include <ansa/module>
#include <ansa/macros>

namespace anarch {

namespace x64 {

class Gdt : public ansa::Module {
public:
  struct Pointer {
    uint16_t limit;
    uint64_t start;
    
    static Pointer GetCurrent(); // @critical
  } ANSA_PACKED;

  static void InitGlobal();
  static Gdt & GetGlobal();
  
  /**
   * Load this GDT on the current CPU.
   * @critical
   */
  void Set();
  
  /**
   * Get the GDT Pointer for this GDT.
   */
  Pointer GetPointer();
  
protected:
  virtual ansa::DepList GetDependencies();
  virtual void Initialize();
  
private:
  void * buffer;
  size_t amountUsed;
};

}

}

#endif
