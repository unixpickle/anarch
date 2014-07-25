#ifndef __ANARCH_API_TIMER_HPP__
#define __ANARCH_API_TIMER_HPP__

#include <anarch/stdint>
#include <ansa/rational>
#include <ansa/macros>
#include <ansa/nocopy>

namespace anarch {

class Timer : public ansa::NoCopy {
public:
  /**
   * @ambicritical
   */
  virtual ansa::Rational<uint64_t> GetTicksPerMicro() = 0;
  
  /**
   * Set a timeout on this timer. This timer MUST be the current timer.
   * @critical
   */
  virtual void SetTimeout(uint64_t ticks, void (* func)()) = 0;
  
  /**
   * Attempts to cancel the current timeout. This timer MUST be the current
   * timer.
   * @discussion This does not have to clear a timeout if it has already fired
   * while in a critical section. When/if this does occur, the timeout may
   * fire directly after the critical section has been exited.
   * @critical
   */
  virtual void ClearTimeout() = 0;
  
  /**
   * Wait for the timer to fire or for a platform-specific interrupt to occur.
   * @critical -> @noncritical (no return)
   */
  virtual void WaitTimeout() ANSA_NORETURN = 0;
};

}

#endif
