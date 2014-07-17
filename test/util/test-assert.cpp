#include <anarch/assert>
#include <anarch/stddef>
#include <stdlib-api/scoped-pass>
#include <stdlib-api/assert>

int main() {
  ScopedPass pass("assert()");
  try {
    assert(true);
    assert(3 == 3);
    try {
      assert(false);
      Die("assert(false) should Panic!");
    } catch (const char * x) {
    }
  } catch (const char * msg) {
    Die(msg);
  }
  return 0;
}
