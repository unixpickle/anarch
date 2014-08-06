#ifndef __ANARCH_API_PAGE_DELEGATE_HPP__
#define __ANARCH_API_PAGE_DELEGATE_HPP__

#include <anarch/types>

namespace anarch {

class PageDelegate {
public:
  /**
   * Get the current system delegate. This will return NULL until a delegate is
   * set.
   * @ambicritical
   */
  static PageDelegate * GetGlobal();
  
  /**
   * Set the current system delegate. You should only call this once, early on
   * in the startup process.
   */
  static void SetGlobal(PageDelegate *);
  
  /**
   * Called when a page fault occurs on a system that supports dynamic memory
   * management.
   */
  virtual void PageFault(VirtAddr address, bool write) = 0;
};

}

#endif
