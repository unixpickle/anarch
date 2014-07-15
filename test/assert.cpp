#include <iostream>
#include <cstdlib>

extern "C" {

int __Assert(const char * msg) {
  std::cerr << msg << std::endl;
  exit(1);
}

}