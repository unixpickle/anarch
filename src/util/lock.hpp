#ifndef __ANARCH_UTIL_LOCK_HPP__
#define __ANARCH_UTIL_LOCK_HPP__

#include <ansa/lock>
#include <anarch/stddef>

namespace anarch {

class Thread;

/**
 * A lock only to be used in critical sections.
 */
class CriticalLock : public ansa::OrderedLock {
public:
  typedef ansa::OrderedLock super;
  virtual void Seize();
  virtual void Release();
};

/**
 * A lock only to be used in non-critical sections.
 */
class NoncriticalLock : public ansa::OrderedLock {
public:
  typedef ansa::OrderedLock super;
  virtual void Seize();
  virtual void Release();
};

using ansa::ScopedLock;

}

#endif
