#ifndef __ANARCH_API_PAGE_DELEGATE_HPP__
#define __ANARCH_API_PAGE_DELEGATE_HPP__

#include <anarch/types>

namespace anarch {

/**
 * A function which may be called to handle a page fault.
 */
typedef void (* PageDelegate)(VirtAddr addr, bool write);

/**
 * Set the global page fault handler. You should call this before initializing
 * any anarch modules.
 * @ambicritical
 */
void SetGlobalPageDelegate(PageDelegate);

/**
 * Returns the global page delegate. If no delegate has been set, this returns
 * `NULL`.
 * @ambicritical
 */
PageDelegate GetGlobalPageDelegate();

}

#endif
