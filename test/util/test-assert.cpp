#include <anarch/assert>
#include <anarch/stddef>
#include "../scoped-pass.hpp"

int main() {
  ScopedPass pass("assert()");
  try {
    assert(true);
    assert(3 == 3);
    try {
      assert(false);
      std::cerr << "assert(false) should Panic!" << std::endl;
      exit(1);
    } catch (const char * x) {
    }
  } catch (const char * msg) {
    std::cerr << "Unexpected Panic: " << msg << std::endl;
    exit(1);
  }
  return 0;
}
