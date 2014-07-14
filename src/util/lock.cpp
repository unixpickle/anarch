#include "lock.hpp"
#include "critical.hpp"

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

// ScopedLock //

ScopedLock::ScopedLock(ansa::Lock & _lock) : lock(_lock) {
  lock.Seize();
}

ScopedLock::~ScopedLock() {
  lock.Release();
}

}
