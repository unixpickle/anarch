#ifndef __ANARCH_X64_STARTUP_CODE_HPP__
#define __ANARCH_X64_STARTUP_CODE_HPP__

#include <anarch/types>

namespace anarch {

namespace x64 {

/**
 * This class configures and destroys trampoline code lower memory for CPU
 * initialization
 */
class StartupCode {
public:
  StartupCode(void (* callbackFunc)());
  ~StartupCode();
  
  void ResetStartupStack();
  
private:
  static const VirtAddr CodeBase = 0x5000;
  static const VirtAddr PageTableBase = 0x3000;
  
  uint64_t callbackFunc;
  
  void * dataBackup;
};

}

}

#endif
