#ifndef __ANARCH_UTIL_IOSTREAM_HPP__
#define __ANARCH_UTIL_IOSTREAM_HPP__

#include <ansa/module>

namespace anarch {

class StreamModule : public ansa::Module {
protected:
  ansa::DepList GetDependencies();
  virtual void Initialize();
};

class OutStream {
public:
  void WriteString(const char * string, size_t length);
};

}

#endif
