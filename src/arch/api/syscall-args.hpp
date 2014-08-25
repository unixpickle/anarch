#ifndef __ANARCH_API_SYSCALL_ARGS_HPP__
#define __ANARCH_API_SYSCALL_ARGS_HPP__

#include <anarch/types>
#include <anarch/stddef>
#include <anarch/stdint>

namespace anarch {

/**
 * A list of arguments passed to a system call. This class must be able to
 * store up to five arguments. If you pop more arguments than were pushed, an
 * undefined value will be returned OR a page fault will be triggered.
 */
class SyscallArgs {
public:
  virtual bool PopBool() = 0;
  virtual int PopInt() = 0;
  virtual uint32_t PopUInt32() = 0;
  virtual PhysAddr PopPhysAddr() = 0;
  virtual VirtAddr PopVirtAddr() = 0;
  virtual PhysSize PopPhysSize() = 0;
  virtual size_t PopVirtSize() = 0;
};

}

#endif
