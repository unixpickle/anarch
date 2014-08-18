# ANArch

This library will provide an interface for Operating System kernels to access architecture-specific/platform-specific features of the target system.

It will provide these APIs by means of abstraction:

 * A set of types equivalent to &lt;cstdint&gt; with additional types for virtual and physical addresses.
 * An API for managing the global address space
 * An API for creating and managing address spaces for user applications
 * A clock interface for accurate timestamps and high-precision notifications
 * A console API for printing (colored) text to the screen
 * A set of functions, classes, and macros for "critical sections"
 * An API which returns a set of "domains"; each domain will include:
   * A physical allocator for allocating memory local to a domain
   * Virtual allocation functions for simple kernel allocations
   * A list of CPUs which run under the domain
 * A CPU-local storage facility
 * A simple function call for handling a panic
 * An API for saving and restoring the state of user-space and kernel-space programs
 * A means by which to sleep the CPU until an asynchronous event occurs.
 * An interface for receiving page faults.
 * An interface for handling system calls.

Additionally, it will provide these tools to wrap the APIs above:

 * A method to easily copy memory from physical memory to virtual memory.
 * An object that maps memory on construction and unmaps it on destruction
 * A set of classes that lock and unlock locks on construction and destruction
 * A class that enters and leaves a critical section on construction and destruction
 * An implementation of the `assert()` function using the panic API
 * A very limited implementation of &lt;iostream&gt;

Architecture-specific APIs will have to be initialized by the operating system's entry point. This means that you may use a custom bootloader, etc. and still use architecture-specific APIs. However, an architecture may provide helper classes/methods for tasks such as parsing multiboot tables.

# TODO

As I work on Alux, I find new things that I need to do in anarch. Here's my immediate TODO list:

 * `AllocPhys()` and `FreePhys()` on Domain for cross-domain fallback
 * A way to re-reserve a chunk of memory with a different page size
 * A way to unmap reserved memory but keep it reserved

I have implemented the following features for x86-64. This pretty much sums up every API that anarch will provide:

 * Physical and virtual memory management
 * The Interrupt Routine Table subsystem
 * ACPI
 * APIC and I/O APIC
 * CPU bootstrap code for SMP initialization
 * A time mechanism with PIT, HPET, and APIC timer support
 * Distributed Panic() and TLB invalidations
 * A `State` implementation for task management
 * User-space memory maps
 * A syscall handler mechanism

Some basic things I'd like to do before v0.1.0

 * Create a `VirtPhysSize` type for sizes that must fit in the physical address space as well as the virtual one.

I have *not* implemented NUMA. In the future, this will be my next step at improving the x86-64 implementation.
