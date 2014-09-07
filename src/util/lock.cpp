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

}
