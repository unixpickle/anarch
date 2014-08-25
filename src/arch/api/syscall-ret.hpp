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
  
  static inline SyscallRet Boolean(bool b) {
    SyscallRet r;
    r.boolean = b;
    return r;
  }
  
  static inline SyscallRet Integer(int i) {
    SyscallRet r;
    r.integer = i;
    return r;
  }
  
  static inline SyscallRet Integer32(uint32_t i) {
    SyscallRet r;
    r.integer32 = i;
    return r;
  }
  
  static inline SyscallRet Phys(PhysAddr p) {
    SyscallRet r;
    r.phys = p;
    return r;
  }
  
  static inline SyscallRet Virt(VirtAddr v) {
    SyscallRet r;
    r.virt = v;
    return r;
  }
  
  static inline SyscallRet PhysSize(PhysSize p) {
    SyscallRet r;
    r.pSize = p;
    return r;
  }
  
  static inline SyscallRet VirtSize(size_t s) {
    SyscallRet r;
    r.vSize = s;
    return r;
  }
};

}

#endif
