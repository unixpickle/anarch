#include "args.hpp"
#include <anarch/assert>

namespace anarch {

namespace x64 {

SyscallArgs::SyscallArgs(uint64_t arg1, uint64_t arg2, uint64_t arg3,
                         uint64_t arg4, uint64_t arg5) {
  arguments[0] = arg1;
  arguments[1] = arg2;
  arguments[2] = arg3;
  arguments[3] = arg4;
  arguments[4] = arg5;
}

bool SyscallArgs::PopBool() {
  return PopUInt64() != 0;
}

int SyscallArgs::PopInt() {
  return (int)PopUInt64();
}

uint32_t SyscallArgs::PopUInt32() {
  return (uint32_t)PopUInt64();
}

uint64_t SyscallArgs::PopUInt64() {
  assert(idx < 5);
  return arguments[idx++];
}

PhysAddr SyscallArgs::PopPhysAddr() {
  return PopUInt64();
}

VirtAddr SyscallArgs::PopVirtAddr() {
  return PopUInt64();
}

PhysSize SyscallArgs::PopPhysSize() {
  return PopUInt64();
}

size_t SyscallArgs::PopVirtSize() {
  return (size_t)PopUInt64();
}

}

}
