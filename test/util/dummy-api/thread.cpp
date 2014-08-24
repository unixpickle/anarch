#include <anarch/api/thread>

namespace anarch {

Thread & Thread::GetCurrent() {
  return *(Thread *)0;
}

}
