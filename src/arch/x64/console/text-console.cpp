#include "text-console.hpp"
#include "../common.hpp"
#include <anarch/critical>
#include <anarch/new>
#include <ansa/cstring>

namespace anarch {

Console & Console::GetGlobal() {
  return x64::TextConsole::GetGlobal();
}

namespace x64 {

namespace {

TextConsole gConsole;

}

void TextConsole::InitGlobal() {
  new(&gConsole) TextConsole();
}

TextConsole & TextConsole::GetGlobal() {
  return gConsole;
}

int TextConsole::GetWidth() {
  return 80;
}

int TextConsole::GetHeight() {
  return 25;
}

uint16_t * TextConsole::GetBuffer() {
  return (uint16_t *)0xb8000;
}

void TextConsole::PrintString(const char * string) {
  ScopedCritical critical;
  ScopedLock scope(lock);
  
  // this is some ugly logic that could probably be simplified
  while (*string) {
    unsigned char theChar = *(string++);
    switch (theChar) {
      case '\n':
        PrintNewline();
        break;
      case '\r':
        PrintCarriageReturn();
        break;
      case '\b':
        PrintBackspace();
        break;
      default:
        PrintCharacter(theChar);
        break;
    }
  }
  if (!(CurrentCharacter() & 0xff00)) {
    CurrentCharacter() = color << 8;
  }
  PositionCursor();
}

void TextConsole::SetColor(Color _color, bool bright) {
  ScopedCritical critical;
  ScopedLock scope(lock);
  color = (uint8_t)_color | (bright ? 8 : 0);
}

ansa::DepList TextConsole::GetDependencies() {
  return ansa::DepList();
}

void TextConsole::Initialize() {
  // zero out the buffer
  ansa::Bzero(GetBuffer(), GetWidth() * GetHeight() * 2);
}

void TextConsole::PrintNewline() {
  x = 0;
  ++y;
  if (y >= GetHeight()) {
    y = GetHeight() - 1;
    ScrollUp();
  }
}

void TextConsole::PrintCarriageReturn() {
  x = 0;
}

void TextConsole::PrintBackspace() {
  if (x != 0) {
    --x;
    CurrentCharacter() = color << 8;
    return;
  }

  // go to the previous line and find the last ASCII character on it
  --y;
  for (x = 0; x < GetWidth(); ++x) {
    if (!(CurrentCharacter() & 0xff)) {
      break;
    }
  }
  if (x == GetWidth()) --x;
  CurrentCharacter() = color << 8;
}

void TextConsole::PrintCharacter(unsigned char theChar) {
  if (x == GetWidth()) {
    PrintNewline();
  }
  CurrentCharacter() = (uint16_t)theChar | (color << 8);
  ++x;
  if (x == 80 && y == GetHeight() - 1) {
    ScrollUp();
    --y;
  }
}

uint16_t TextConsole::GetBufferIndex() {
  uint16_t pos = (uint16_t)((y * GetWidth()) + x);
  if (pos >= GetWidth() * GetHeight()) {
    pos = GetWidth() * GetHeight() - 1;
  }
  return pos;
}

uint16_t & TextConsole::CurrentCharacter() {
  return GetBuffer()[GetBufferIndex()];
}

void TextConsole::PositionCursor() {
  uint16_t position = GetBufferIndex();
  
  // tell the VGA index register we are sending the `low` byte
  x64::OutB(0x3D4, 0x0f);
  x64::OutB(0x3D5, (unsigned char)(position & 0xff));
  // and now send the `high` byte
  x64::OutB(0x3D4, 0x0e);
  x64::OutB(0x3D5, (unsigned char)((position >> 8) & 0xff));
}

void TextConsole::ScrollUp() {
  uint16_t * buffer = GetBuffer();
  
  // copy the buffer into itself, one line up
  int i;
  for (i = 0; i < GetWidth() * (GetHeight() - 1); i++) {
    buffer[i] = buffer[i + GetWidth()];
  }
  
  // clear the bottom line
  for (; i < GetWidth() * GetHeight(); i++) {
    buffer[i] = 0;
  }
}

}

}
