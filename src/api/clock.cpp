#include "clock.hpp"
#include <anarch/critical>

namespace anarch {

uint64_t Clock::GetMicros() {
  return GetMicrosPerTick().ScaleInteger(GetTicks());
}

void Clock::WaitUntil(uint64_t ticks) {
  AssertNoncritical();
  while (GetTicks() < ticks);
}

void Clock::Sleep(uint64_t ticks) {
  WaitUntil(GetTicks() + ticks);
}

void Clock::MicroSleep(uint64_t micros) {
  uint64_t now = GetTicks();
  uint64_t tickCount = GetMicrosPerTick().Flip().ScaleInteger(micros);
  WaitUntil(now + tickCount);
}

}
