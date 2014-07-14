#include "stream.hpp"
#include <anarch/api/console>
#include <anarch/new>

namespace anarch {

static StreamModule gModule;
ConsoleOutStream cout;
ConsoleErrorStream cerr;
const char * endl = "\n";

void StreamModule::InitGlobal() {
  new(&gModule) StreamModule();
}

StreamModule & StreamModule::GetGlobal() {
  return gModule;
}
  
ansa::DepList StreamModule::GetDependencies() {
  return ansa::DepList(&Console::GetGlobal());
}

void StreamModule::Initialize() {
  new(&cout) ConsoleOutStream();
  new(&cerr) ConsoleErrorStream();
}

void ConsoleOutStream::Puts(const char * str) {
  console.SetColor(Console::ColorLightGray, true);
  console.PrintString(str);
}

void ConsoleErrorStream::Puts(const char * str) {
  console.SetColor(Console::ColorRed, true);
  console.PrintString(str);
}

OutStream & operator<<(OutStream & stream, unsigned char num) {
  return stream << (unsigned long long)num;
}

OutStream & operator<<(OutStream & stream, unsigned short num) {
  return stream << (unsigned long long)num;
}

OutStream & operator<<(OutStream & stream, unsigned int num) {
  return stream << (unsigned long long)num;
}

OutStream & operator<<(OutStream & stream, unsigned long num) {
  return stream << (unsigned long long)num;
}

OutStream & operator<<(OutStream & stream, unsigned long long number) {
  const char * chars = "0123456789abcdef";
  unsigned char _buf[32] = {0};
  _buf[0] = '0';
  _buf[1] = 'x';
  unsigned char * buf = _buf + 2;
  unsigned char len = 2, i;
  do {
    unsigned char nextDig = (unsigned char)(number & 0xf);
    buf[len++] = chars[nextDig];
    number >>= 4;
  } while (number > 0);
  for (i = 0; i < len / 2; i++) {
    unsigned char a = buf[len - i - 1];
    buf[len - i - 1] = buf[i];
    buf[i] = a;
  }
  stream.Puts(_buf);
  return stream;
}

OutStream & operator<<(OutStream & stream, const char * str) {
  stream.Puts(str);
}

}