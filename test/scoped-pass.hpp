#ifndef __TEST_SCOPED_PASS_HPP__
#define __TEST_SCOPED_PASS_HPP__

extern "C" {
extern int printf(const char * format, ...);
extern void fflush(void *);
}

class ScopedPass {
public:
  template <typename... Y>
  inline ScopedPass(Y... args) {
    printf("testing");
    fflush(NULL);
    Initializer(args...);
  }
  
  inline ~ScopedPass() {
    printf("passed!\n");
  }
  
  inline void Initializer() {
    printf(" ... ");
    fflush(NULL);
  }
  
  template <typename X, typename... Y>
  inline void Initializer(X arg1, Y... args) {
    printf(" %s", arg1);
    Initializer(args...);
  }
};

#endif
