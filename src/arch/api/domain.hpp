#ifndef __ANARCH_API_DOMAIN_HPP__
#define __ANARCH_API_DOMAIN_HPP__

#include "thread.hpp"
#include "allocator.hpp"
#include "virtual-allocator.hpp"
#include <anarch/stddef>
#include <ansa/nocopy>

namespace anarch {

class Domain : public ansa::NoCopy {
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
  
  virtual bool Alloc(void *&, size_t); // @noncritical
  virtual void Free(void *); // @noncritical
  
  /**
   * Allocate an object, preferably from this Domain's allocator.
   * @noncritical
   */
  template <typename T, typename... Args>
  T * New(Args... args) {
    void * buf;
    if (!Alloc(buf, sizeof(T))) return NULL;
    return new(buf) T(args...);
  }
  
  /**
   * Delete an object which was most likely allocated on this Domain's
   * allocator.
   * @noncritical
   */
  template <typename T>
  void Delete(T * ptr) {
    ptr->~T();
    Free((void *)ptr);
  }
};

}

#endif
