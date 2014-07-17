#include <iostream>
#include <cstdlib>

extern "C" {

int __stdlib_api_assert(const char * msg) {
  std::cerr << msg << std::endl;
  exit(1);
}

}
