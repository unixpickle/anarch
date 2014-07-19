#ifndef __ANARCH_X64_TEXT_CONSOLE_HPP__
#define __ANARCH_X64_TEXT_CONSOLE_HPP__

#include <anarch/api/console>
#include <anarch/lock>

namespace anarch {

namespace x64 {

/**
 * Simple, temporary implementation of the text-mode console. I will use this
 * until I come up with a better set of abstractions to support graphics modes.
 */
class TextConsole : public Console {
public:
  static void InitGlobal();
  static TextConsole & GetGlobal();
  
  virtual int GetWidth();
  virtual int GetHeight();
  
  virtual void PrintString(const char * string);
  virtual void SetColor(Color color, bool bright);
  
protected:
  virtual ansa::DepList GetDependencies();
  virtual void Initialize();
  
private:
  uint8_t color = 0xa;
  int x = 0, y = 0;
  uint64_t * buffer = (uint64_t *)0xb8000;
  CriticalLock lock;
  
  virtual void MoveCursor();
  virtual void ScrollUp();
};

}

}

#endif
