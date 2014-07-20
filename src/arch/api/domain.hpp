#ifndef __ANARCH_API_DOMAIN_HPP__
#define __ANARCH_API_DOMAIN_HPP__

#include "thread.hpp"
#include "allocator.hpp"
#include "virtual-allocator.hpp"
#include <anarch/stddef>

namespace anarch {

class Domain {
public:
  /**
   * Get the current domain of the current hardware thread.
   * @critical
   */
  static Domain & GetCurrent();
  
  /**
   * This is another way to access the list of Domains. This approach returns
   * Domains in order of "closeness" to this Domain. If every Domain is the
   * same "distance" away, the order is irrelevant.
   * @ambicritical
   */
  virtual Domain & GetSibling(int idx) = 0;
  
  virtual int GetThreadCount() = 0; // @ambicritical
  virtual Thread & GetThread(int idx) = 0; // @ambicritical
  
  virtual Allocator & GetAllocator() = 0; // @ambicritical
  virtual VirtualAllocator & GetVirtualAllocator() = 0; // @ambicritical
  
  virtual bool Alloc(void *&, size_t);
  virtual void Free(void *);
};



#endif
