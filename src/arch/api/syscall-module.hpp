#ifndef __ANARCH_API_SYSCALL_MODULE_HPP__
#define __ANARCH_API_SYSCALL_MODULE_HPP__

#include "syscall-args.hpp"
#include "syscall-ret.hpp"
#include <ansa/module>

namespace anarch {

class SyscallModule : public ansa::Module {
public:
  typedef SyscallRet (* SyscallHandler)(uint16_t number, SyscallArgs & args);
  
  static SyscallModule & GetGlobal(); // @ambicritical
  
  /**
   * Set the global syscall handler. You must call this before any user-space
   * task triggers a system call.
   * @ambicritical
   */
  virtual void SetHandler(SyscallHandler handler) = 0;
};

}

#endif
