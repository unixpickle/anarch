#include <anarch/api/syscall-args>

namespace anarch {

namespace x64 {

class SyscallArgs : public anarch::SyscallArgs {
public:
  SyscallArgs(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4,
              uint64_t arg5);
  
  virtual int MaxArgCount();
  virtual bool PopBool();
  virtual int PopInt();
  virtual uint32_t PopUInt32();
  virtual uint64_t PopUInt64();
  virtual PhysAddr PopPhysAddr();
  virtual VirtAddr PopVirtAddr();
  virtual PhysSize PopPhysSize();

private:
  uint64_t arguments[5];
  int idx = 0;
};

}

}
