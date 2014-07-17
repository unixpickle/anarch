#ifndef __ANARCH_DUMMY_CONSOLE_HPP__
#define __ANARCH_DUMMY_CONSOLE_HPP__

#include <anarch/api/console>
#include <anarch/stddef>

namespace anarch {

namespace dummy {

class LogConsole : public Console {
public:
  static const size_t Capacity = 25 * 80;
  
  static LogConsole & GetGlobal(); // no need for InitGlobal on real platform
  
  virtual void PrintString(const char * str);
  
  virtual void SetColor(Color, bool) {
  }
  
  inline char * GetBacklog() {
    return backlog;
  }
  
  inline size_t GetBacklogSize() {
    return backlogSize;
  }
  
protected:
  char backlog[Capacity];
  size_t backlogSize = 0;
  
  virtual ansa::DepList GetDependencies() {
    return ansa::DepList();
  }
  
  virtual void Initialize();
};

}

}

#endif
