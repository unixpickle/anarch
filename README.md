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

# Usage

**anarch** is almost completely stand-alone. However, it does include one header that it does not provide. You may notice this line of code in [src/util/lock.hpp](src/util/lock.hpp).

    #include <anarch/noncritical-lock>

The API consumer is responsible for providing this header file. The header should define a class `anarch::NoncriticalLock` that is a subclass of `ansa::Lock`. This class will be used throughout anarch for locking various internal structures.

Your operating system should provide an implementation of `anarch::NoncriticalLock` that yields to waiting tasks to prevent unnecessary CPU usage.

# TODO

Although anarch is up and running for x86-64, I would still like to rewrite it before I add more architectures. Here is my current wish-list for the rewrite:

 * Get rid of singletons and "Modules" -- replace with top-level functions
 * Remove most of the generic API base-classes; instead, architecture specific code will provide its own includes and classes. This will allow for better optimization.
 * ASID (address-space identifier) support for modern CPUs
 * Support non-uniform memory access for the x86-64 platform
 * Better scalable virtual memory management
   * Slabs
   * Per-CPU memory pools
 * Time stamp counter support for CPUs with invariant TSC
 * `Print()` and `Debug()` functions instead of `cout`.
 * Performance monitoring to detect how much work a hyperthread (on x86-64) is performing.
