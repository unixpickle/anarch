#include "thread.hpp"

namespace anarch {

void * Thread::GetUserInfo() {
  return Thread::GetCurrent().userInfo;
}

void Thread::SetUserInfo(void * info) {
  Thread::GetCurrent().userInfo = info;
}

}
