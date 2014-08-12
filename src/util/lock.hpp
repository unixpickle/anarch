#ifndef __ANARCH_UTIL_LOCK_HPP__
#define __ANARCH_UTIL_LOCK_HPP__

#include <ansa/lock>
#include <anarch/stddef>

namespace anarch {

class Thread;

/**
 * A lock only to be used in critical sections.
 */
class CriticalLock : public ansa::Lock {
public:
  typedef ansa::Lock super;
  virtual void Seize();
  virtual void Release();
  virtual void SeizeYielding();
};

/**
 * A lock only to be used in non-critical sections.
 */
class NoncriticalLock : public ansa::Lock {
public:
  typedef ansa::Lock super;
  virtual void Seize();
  virtual void Release();
  virtual void SeizeYielding();
};

class RecursiveLock : public ansa::Lock {
public:
  typedef ansa::Lock super;
  virtual void Seize(); // @critical
  virtual void Release(); // @critical
  virtual void SeizeYielding(); // @critical
  
private:
  CriticalLock holdingLock;
  Thread * holding = NULL;
  int holdCount = 0;
};

class ScopedLock {
public:
  ScopedLock(ansa::Lock & lock);
  ~ScopedLock();
  
private:
  ansa::Lock & lock;
};

}

#endif
