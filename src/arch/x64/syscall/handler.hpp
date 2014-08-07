#ifndef __ANARCH_X64_SYSCALL_HANDLER_HPP__
#define __ANARCH_X64_SYSCALL_HANDLER_HPP__

/**
 * The entry point of the syscall handler.
 * @critical
 */
void AnarchRawSyscallHandler() __asm__("AnarchRawSyscallHandler");

#endif
