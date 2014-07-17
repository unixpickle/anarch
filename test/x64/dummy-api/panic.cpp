#include <anarch/api/panic>

namespace anarch {

void Panic(const char * str) {
  throw str;
}

}
