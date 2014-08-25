global AnarchRawSyscallHandler
extern AnarchSyscallMainEntry

AnarchRawSyscallHandler:
  swapgs ; get our CPU data section
  mov r10, rsp ; save the user stack
  mov rsp, [gs:0x8] ; syscallStack field
  
  ; push the old user stack pointer and the return address
  push r10
  push rcx
  
  ; set the 4th argument since this register was used by SYSCALL
  mov rcx, rbx
  
  ; standard System V ABI function; saves some regs for us
  call AnarchSyscallMainEntry ; sets RAX to return value
  
  pop rcx
  pop rsp
  
  mov r11, 0x200
  ; TODO: verify that RCX is canonical! THIS IS A SECURITY ISSUE
  
  swapgs
  o64 sysret
