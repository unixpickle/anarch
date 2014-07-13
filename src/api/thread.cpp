#include "thread.hpp"

namespace anarch {

static void * Thread::GetUserInfo() {
  return Thread::GetCurrent().userInfo;
}

static void Thread::SetUserInfo(void * info) {
  Thread::GetCurrent().userInfo = info;
}

}
