#ifndef __TEST_SCOPED_PASS_HPP__
#define __TEST_SCOPED_PASS_HPP__

#include <iostream>

class ScopedPass {
public:
  template <class... Y>
  inline ScopedPass(Y... args) {
    std::cout << "testing ";
    Initializer(args...);
  }
  
  inline ~ScopedPass() {
    std::cout << "passed!" << std::endl;
  }
  
  inline void Initializer() {
    std::cout << " ... " << std::flush;
  }
  
  template <class X, class... Y>
  inline void Initializer(X arg1, Y... args) {
    std::cout << arg1;
    Initializer(args...);
  }
};

#endif
