#ifndef __ANARCH_X64_SYSCALL_MODULE_HPP__
#define __ANARCH_X64_SYSCALL_MODULE_HPP__

#include <anarch/api/syscall-module>
#include <anarch/stddef>
#include <ansa/atomic>

anarch::SyscallRet AnarchSyscallMainEntry(uint64_t, uint64_t, uint64_t,
                                          uint64_t, uint64_t, uint64_t)
                                         __asm__("AnarchSyscallMainEntry");

namespace anarch {

namespace x64 {

class SyscallModule : public anarch::SyscallModule {
public:
  static const uint32_t MsrSTAR = 0xC0000081;
  static const uint32_t MsrLSTAR = 0xC0000082;
  static const uint32_t MsrSFMask = 0xC0000084;
  
  static void InitGlobal();
  static SyscallModule & GetGlobal();
  
  virtual void SetHandler(SyscallHandler handler);
  SyscallHandler GetHandler();
  
protected:
  virtual ansa::DepList GetDependencies();
  virtual void Initialize();
  
private:
  SyscallHandler handler = NULL;
  ansa::Atomic<int> initCount;
  
  static void SetCpuRegisters();
};

}

}

#endif
