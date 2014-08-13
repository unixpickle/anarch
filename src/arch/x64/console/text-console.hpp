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
  
  int GetWidth();
  int GetHeight();
  uint16_t * GetBuffer();
  
  virtual void PrintString(const char * string);
  virtual void SetColor(Color color, bool bright);
  
protected:
  virtual ansa::DepList GetDependencies();
  virtual void Initialize();
  
private:
  uint8_t color = 0xa;
  int x = 0;
  int y = 0;
  CriticalLock lock;
  
  void PrintNewline();
  void PrintCarriageReturn();
  void PrintBackspace();
  void PrintCharacter(unsigned char theChar);
  uint16_t GetBufferIndex();
  uint16_t & CurrentCharacter();
  
  void PositionCursor();
  void ScrollUp();
};

}

}

#endif
