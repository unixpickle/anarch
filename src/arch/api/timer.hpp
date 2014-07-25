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
   * Set a timeout on this timer WHICH MUST BE THE CURRENT TIMER.
   * @critical
   */
  virtual void SetTimeout(uint64_t ticks, void (* func)()) = 0;
  
  /**
   * Cancel this timer WHICH MUST BE THE CURRENT TIMER.
   * @discussion If a timeout has already fired but has not yet been handled,
   * calling this method will suppress the call to the handler.
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
