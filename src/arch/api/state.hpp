#ifndef __ANARCH_API_STATE_HPP__
#define __ANARCH_API_STATE_HPP__

#include <ansa/macros>
#include <ansa/nocopy>

namespace anarch {

class State : public ansa::NoCopy {
public:
  // all @noncritical
  static State & NewKernel(void (*)());
  static State & NewKernel(void (*)(void *), void *);
  static State & NewUser(void (*)());
  static State & NewUser(void (*)(void *), void *);
  
  virtual void Delete() = 0; // @noncritical
  
  virtual void Resume() ANSA_NORETURN = 0; // @critical
  virtual void SuspendAndCall(void (*)()) = 0; // @critical
  virtual void SuspendAndCall(void (*)(void *), void *) = 0; // @critical
  
protected:
  virtual ~State() {}
};

}

#endif
