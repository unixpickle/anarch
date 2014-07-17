#include <iostream>
#include <cstdlib>

extern "C" {

int Die(const char * msg) {
  std::cerr << msg << std::endl;
  exit(1);
}

}
