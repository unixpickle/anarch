#ifndef __ANARCH_API_SYSCALL_ARGS_HPP__
#define __ANARCH_API_SYSCALL_ARGS_HPP__

#include <anarch/types>
#include <anarch/stddef>
#include <anarch/stdint>

namespace anarch {

class SyscallArgs {
public:
  virtual int MaxArgCount() = 0;
  
  virtual bool PopBool() = 0;
  virtual int PopInt() = 0;
  virtual uint32_t PopUInt32() = 0;
  virtual uint64_t PopUInt64() = 0;
  virtual PhysAddr PopPhysAddr() = 0;
  virtual VirtAddr PopVirtAddr() = 0;
  virtual PhysSize PopPhysSize() = 0;
  virtual size_t PopVirtSize() = 0;
};

}

#endif
