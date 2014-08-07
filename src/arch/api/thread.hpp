#ifndef __ANARCH_API_THREAD_HPP__
#define __ANARCH_API_THREAD_HPP__

#include <ansa/nocopy>

namespace anarch {

class Domain;
class Timer;

/**
 * Represents a hardware thread, not a software thread!
 */
class Thread : public ansa::NoCopy {
public:
  // implemented in Thread.cpp
  static void * GetUserInfo(); // @critical
  static void SetUserInfo(void *); // @critical
  
  static Thread & GetCurrent(); // @critical
  
  virtual Domain & GetDomain() = 0; // @ambicritical
  
  virtual Timer & GetTimer() = 0; // @critical
  
  /**
   * Return a domain-local priority for this thread. This way, you can make one
   * core for each physical CPU a higher priority than the next, etc., and
   * influence some sort of scheduler to avoid using hyperthreads unless it is
   * needed.
   * @ambicritical
   */
  virtual int GetPriority() = 0;
  
  /**
   * Asynchronously send this Thread a message by causing it to run a function.
   * If this is called multiple times before the thread receives a message, the
   * thread may not receive every message, but it will receive at least one.
   * 
   * The specified function will run in a critical section.
   *
   * @critical
   */
  virtual void RunAsync(void (*)()) = 0;
};

}

#endif
