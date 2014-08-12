#include "lock.hpp"
#include "critical.hpp"
#include <anarch/api/thread>

namespace anarch {

// CriticalLock //

void CriticalLock::Seize() {
  AssertCritical();
  super::Seize();
}

void CriticalLock::Release() {
  AssertCritical();
  super::Release();
}

void CriticalLock::SeizeYielding() {
  AssertCritical();
  super::SeizeYielding();
}

// NoncriticalLock //

void NoncriticalLock::Seize() {
  AssertNoncritical();
  super::Seize();
}

void NoncriticalLock::Release() {
  AssertNoncritical();
  super::Release();
}

void NoncriticalLock::SeizeYielding() {
  AssertNoncritical();
  super::SeizeYielding();
}

// RecursiveLock //

void RecursiveLock::Seize() {
  AssertCritical();
  holdingLock.Seize();
  if (holding == &Thread::GetCurrent()) {
    ++holdCount;
    holdingLock.Release();
  } else {
    holdingLock.Release();
    super::Seize();
    
    ScopedLock scope(holdingLock);
    holding = &Thread::GetCurrent();
    assert(holdCount == 0);
    holdCount = 1;
  }
}

void RecursiveLock::Release() {
  AssertCritical();
  ScopedLock scope(holdingLock);
  if (holding == &Thread::GetCurrent()) {
    if (!--holdCount) {
      super::Release();
      holding = NULL;
    }
  } else {
    super::Release();
  }
}

void RecursiveLock::SeizeYielding() {
  AssertCritical();
  holdingLock.Seize();
  if (holding == &Thread::GetCurrent()) {
    ++holdCount;
    holdingLock.Release();
  } else {
    holdingLock.Release();
    super::SeizeYielding();
  
    ScopedLock scope(holdingLock);
    holding = &Thread::GetCurrent();
    assert(holdCount == 0);
    holdCount = 1;
  }
}

// ScopedLock //

ScopedLock::ScopedLock(ansa::Lock & _lock) : lock(_lock) {
  lock.Seize();
}

ScopedLock::~ScopedLock() {
  lock.Release();
}

}
