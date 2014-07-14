#include "critical.hpp"

namespace anarch {

ScopedCritical::ScopedCritical() : wasCritical(GetCritical()) {
  if (!wasCritical) SetCritical(true);
}

ScopedCritical::~ScopedCritical() {
  if (!wasCritical) SetCritical(false);
}

}
