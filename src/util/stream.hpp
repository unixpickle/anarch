#ifndef __ANARCH_UTIL_STREAM_HPP__
#define __ANARCH_UTIL_STREAM_HPP__

#include <ansa/module>

namespace anarch {

class StreamModule : public ansa::Module {
public:
  static void InitGlobal();
  static StreamModule & GetGlobal();
  
protected:
  ansa::DepList GetDependencies();
  virtual void Initialize();
};

class OutStream {
public:
  virtual void Puts(const char * string) = 0;
};

class ConsoleOutStream : public OutStream {
public:
  virtual void Puts(const char * string);
};

class ConsoleErrorStream : public OutStream {
public:
  virtual void Puts(const char * string);
};

OutStream & operator<<(OutStream &, unsigned char);
OutStream & operator<<(OutStream &, unsigned short);
OutStream & operator<<(OutStream &, unsigned int);
OutStream & operator<<(OutStream &, unsigned long);
OutStream & operator<<(OutStream &, unsigned long long);
OutStream & operator<<(OutStream &, const char *);

extern ConsoleOutStream cout;
extern ConsoleErrorStream cerr;
extern const char * endl;

}

#endif
