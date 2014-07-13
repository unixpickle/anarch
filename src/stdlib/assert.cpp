#include <anarch/panic>

extern "C" {

int __assert(const char * msg) {
  anarch::Panic(msg);
  return 0;
}

}
