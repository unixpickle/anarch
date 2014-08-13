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

void TextConsole::PrintString(const char * string) {
  ScopedCritical critical;
  ScopedLock scope(lock);
  
  // this is some ugly logic that could probably be simplified
  while (*string) {
    unsigned char theChar = *(string++);
    if (theChar == '\n') {
      if (x != 0) {
        y++;
        x = 0;
      }
    } else if (theChar == '\r') {
      x = 0;
      continue;
    } else if (theChar == '\b') {
      if (x == 0) {
        if (y != 0) {
          y--;
          // find the last character on this line
          x = GetWidth() - 1;
          int i, newLoc = (GetWidth() * y);
          for (i = 0; i < GetWidth(); i++) {
            if (!buffer[newLoc + i]) {
              x = i;
              break;
            }
          }
        }
      } else x--;
      int loc = x + (GetWidth() * y);
      buffer[loc] = color << 8;
    } else {
      int loc = x + (GetWidth() * y);
      buffer[loc] = (uint16_t)theChar | (color << 8);
      x++;
    }
    if (x >= GetWidth()) {
      x = 0;
      y++;
    }
    while (y >= GetHeight()) {
      ScrollUp();
      y--;
    }
  }
  MoveCursor();
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
  ansa::Bzero(buffer, GetWidth() * GetHeight() * 2);
}

void TextConsole::MoveCursor() {
  uint16_t position = (uint16_t)((y * GetWidth()) + x);
  
  // tell the VGA index register we are sending the `low` byte
  x64::OutB(0x3D4, 0x0f);
  x64::OutB(0x3D5, (unsigned char)(position & 0xff));
  // and now send the `high` byte
  x64::OutB(0x3D4, 0x0e);
  x64::OutB(0x3D5, (unsigned char)((position >> 8) & 0xff));

  buffer[position] = (uint16_t)color << 8;
}

void TextConsole::ScrollUp() {
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
