#ifndef __ANARCH_API_SYSCALL_RET_HPP__
#define __ANARCH_API_SYSCALL_RET_HPP__

#include <anarch/types>
#include <anarch/assert>
#include <anarch/stddef>
#include <anarch/stdint>

namespace anarch {

/**
 * The return value for a system call. This may be an error value.
 */
struct SyscallRet {
  union {
    bool boolean;
    int integer;
    uint32_t integer32;
    uint64_t integer64;
    PhysAddr phys;
    VirtAddr virt;
    PhysSize pSize;
    size_t vSize;
  } value;
  
  int errorValue = 0;
  
  static inline SyscallRet Error(int code) {
    assert(code != 0);
    SyscallRet r;
    r.errorValue = code;
    return r;
  }
  
  static inline SyscallRet Boolean(bool b) {
    SyscallRet r;
    r.value.boolean = b;
    return r;
  }
  
  static inline SyscallRet Integer(int i) {
    SyscallRet r;
    r.value.integer = i;
    return r;
  }
  
  static inline SyscallRet Integer32(uint32_t i) {
    SyscallRet r;
    r.value.integer32 = i;
    return r;
  }
  
  static inline SyscallRet Integer64(uint64_t i) {
    SyscallRet r;
    r.value.integer64 = i;
    return r;
  }
  
  static inline SyscallRet Phys(PhysAddr p) {
    SyscallRet r;
    r.value.phys = p;
    return r;
  }
  
  static inline SyscallRet Virt(VirtAddr v) {
    SyscallRet r;
    r.value.virt = v;
    return r;
  }
  
  static inline SyscallRet PhysSize(PhysSize p) {
    SyscallRet r;
    r.value.pSize = p;
    return r;
  }
  
  static inline SyscallRet VirtSize(size_t s) {
    SyscallRet r;
    r.value.vSize = s;
    return r;
  }
};

}

#endif
