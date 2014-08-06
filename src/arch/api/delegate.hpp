#ifndef __ANARCH_API_DELEGATE_HPP__
#define __ANARCH_API_DELEGATE_HPP__

#include "syscall-args.hpp"
#include <anarch/types>

namespace anarch {

class Delegate {
public:
  /**
   * Get the current system delegate. This will return NULL until a delegate is
   * set.
   * @ambicritical
   */
  static Delegate * GetGlobalDelegate();
  
  /**
   * Set the current system delegate. You should only call this once, early on
   * in the startup process.
   */
  static void SetGlobalDelegate(Delegate *);
  
  /**
   * Called when a page fault occurs on a system that supports dynamic memory
   * management.
   */
  virtual void PageFault(VirtAddr address, bool write) = 0;
  
  /**
   * Called when a user-space task performs a system call.
   */
  virtual void Syscall(const SyscallArgs & args);
};

}

#endif
