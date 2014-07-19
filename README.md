# ANArch

This library will provide an interface for Operating System kernels to access architecture-specific/platform-specific features of the target system.

It will provide these APIs by means of abstraction:

 * A set of types equivalent to &lt;cstdint&gt; with additional types for virtual and physical addresses.
 * An API for managing the global address space
 * An API for creating and managing address spaces for user applications
 * A clock interface for accurate timestamps and high-precision notifications
 * A console API for printing (colored) text to the screen
 * A set of functions for "critical sections"
 * An API which returns a set of "domains"; each domain will include:
   * A physical allocator for allocating memory local to a domain
   * A list of CPUs which run under a domain
 * A CPU-local storage facility
 * A simple function call for handling a panic
 * An API for saving and restoring the state of user-space and kernel-space programs
 * A means by which to sleep the CPU until a timer event occurs.
 * An interface for receiving page faults.

Additionally, it will provide these tools to wrap the APIs above:

 * A method to easily copy memory from physical memory to virtual memory.
 * An object that maps memory on construction and unmaps it on destruction
 * A set of classes that lock and unlock locks on construction and destruction
 * A class that enters and leaves a critical section on construction and destruction
 * An implementation of the `assert()` function using the panic API
 * A very limited implementation of &lt;iostream&gt;

Architecture-specific APIs will have to be initialized by the operating system's entry point. This means that you may use a custom bootloader, etc. and still use architecture-specific APIs. However, an architecture may provide helper classes/methods for tasks such as parsing multiboot tables.

# TODO

Right now, I am working on getting memory management up and running under Alux on real x86-64 hardware (and, of course, a VM).

 * Implement Panic() x86-64 API
 * Implement critical x86-64 API
 * Implement console x86-64 API
 * Test what we have so far on real hardware!
 * Pull in the existing Alux source piece by piece
   * ACPI subsystem
   * Interrupt subsystem
 * Implement EasyMap and other utilities
