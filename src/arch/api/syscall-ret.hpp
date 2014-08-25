#ifndef __ANARCH_API_SYSCALL_RET_HPP__
#define __ANARCH_API_SYSCALL_RET_HPP__

#include <anarch/types>
#include <anarch/stddef>
#include <anarch/stdint>

namespace anarch {

/**
 * The return value for a system call.
 */
union SyscallRet {
  bool boolean;
  int integer;
  uint32_t integer32;
  PhysAddr phys;
  VirtAddr virt;
  PhysSize pSize;
  size_t vSize;
};

}

#endif
