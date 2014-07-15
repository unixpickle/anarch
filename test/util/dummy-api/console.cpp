#include "console.hpp"
#include <ansa/cstring>

namespace anarch {

Console & Console::GetGlobal() {
  return dummy::LogConsole::GetGlobal();
}

namespace dummy {

namespace {

LogConsole gConsole;

}

LogConsole & LogConsole::GetGlobal() {
  return gConsole;
}

void LogConsole::PrintString(const char * str) {
  size_t length = ansa::Strlen(str);
  size_t remaining = Capacity - backlogSize;
  if (length <= remaining) {
    // perfect
    ansa::Memcpy(&backlog[backlogSize], str, length);
    backlogSize += length;
  } else if (length <= Capacity) {
    // we just need to make room
    size_t subtractSize = length - remaining;
    for (size_t i = 0; i < backlogSize - subtractSize; i++) {
      backlog[i] = backlog[i + subtractSize];
    }
    backlogSize -= subtractSize;
    PrintString(str);
  } else {
    // just copy the end of the string to the buffer
    size_t offset = length - Capacity;
    for (size_t i = 0; i < Capacity; i++) {
      backlog[i] = str[i + offset];
    }
    backlogSize = Capacity;
  }
}

void LogConsole::Initialize() {
  ansa::Bzero(backlog, 80 * 25);
}

}

}
