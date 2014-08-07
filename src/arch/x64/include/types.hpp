#ifndef __ANARCH_X64_TYPES_HPP__
#define __ANARCH_X64_TYPES_HPP__

#include <anarch/stdint>

/**
 * A numerical representation of a physical address.
 */
typedef uint64_t PhysAddr;

/**
 * A numerical representation of the size of a chunk of physical memory.
 */
typedef uint64_t PhysSize;

/**
 * A numerical representation of a virtual address.
 */
typedef uint64_t VirtAddr;

#endif
