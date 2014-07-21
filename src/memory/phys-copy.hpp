#ifndef __ANARCH_PHYS_COPY_HPP__
#define __ANARCH_PHYS_COPY_HPP__

#include <anarch/stddef>
#include <anarch/types>

namespace anarch {

/**
 * Temporarily maps a physical address to the virtual address space using an
 * EasyMap, then copies that memory, then unmaps it.
 * @noncritical
 */
void PhysCopy(void * dest, PhysAddr source, size_t size);

}

#endif
