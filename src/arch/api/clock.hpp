#ifndef __ANARCH_API_CLOCK_HPP__
#define __ANARCH_API_CLOCK_HPP__

#include <anarch/stdint>
#include <ansa/rational>
#include <ansa/nocopy>

namespace anarch {

class Clock : public ansa::NoCopy {
public:
  /**
   * Returns the time in "ticks"
   * @ambicritical
   */
  virtual uint64_t GetTicks() = 0;
  
  /**
   * Returns a ratio: micros/tick
   * @ambicritical
   */
  virtual ansa::Rational<uint64_t> GetMicrosPerTick() = 0;
  
  /**
   * Uses the ratio from GetMicrosPerTick() with GetTicks().
   * @ambicritical
   */
  virtual uint64_t GetMicros();
  
  /**
   * Sleep until GetTicks() returns at least a certain value.
   * @noncritical
   */
  virtual void WaitUntil(uint64_t ticks);
  
  /**
   * Sleep for a certain number of ticks.
   * @noncritical
   */
  virtual void Sleep(uint64_t ticks);
 
  /**
   * Sleep for a certain number of microseconds.
   * @noncritical
   */
  virtual void MicroSleep(uint64_t micros);
  
};

}

#endif
