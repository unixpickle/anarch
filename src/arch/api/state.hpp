#ifndef __ANARCH_API_STATE_HPP__
#define __ANARCH_API_STATE_HPP__

namespace anarch {

class State {
public:
  static State & NewKernel(void (*)());
  static State & NewKernel(void (*)(void *), void *);
  static State & NewUser(void (*)());
  static State & NewUser(void (*)(void *), void *);
  
  virtual void Delete() = 0;
  
  virtual void Resume() = 0;
  virtual void SuspendAndCall(void (*)()) = 0;
  
protected:
  virtual ~State() {}
};

}

#endif
