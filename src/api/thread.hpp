#ifndef __ANARCH_API_THREAD_HPP__
#define __ANARCH_API_THREAD_HPP__

namespace anarch {

class Domain;

/**
 * Represents a hardware thread, not a software thread!
 */
class Thread {
public:
  // implemented in Thread.cpp
  static void * GetUserInfo(); // @critical
  static void SetUserInfo(void *); // @critical
  
  static Thread & GetCurrent(); // @critical
  
  virtual Domain & GetDomain() = 0; // @ambicritical
  
  /**
   * Return a domain-local priority for this thread. This way, you can make one
   * core for each physical CPU a higher priority than the next, etc., and
   * influence some sort of scheduler to avoid using hyperthreads unless it is
   * needed.
   * @ambicritical
   */
  virtual int GetPriority() = 0;
  
private:
  void * userInfo;
};

}

#endif
