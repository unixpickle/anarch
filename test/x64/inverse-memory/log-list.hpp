#ifndef __ANARCH_TEST_X64_LOG_LIST_HPP__
#define __ANARCH_TEST_X64_LOG_LIST_HPP__

#include <cassert>
#include <cstddef>

namespace anarch {

namespace dummy {

template <class T>
class LogList {
public:
  int GetCount() {
    return logCount;
  }
  
  const T & operator[](int idx) {
    assert(idx < logCount);
    T * entry = logFirst;
    for (int i = 0; i < idx; i++) {
      entry = entry->logNext;
    }
    return *entry;
  }
  
  void Clear() {
    T * entry = logFirst;
    while (entry) {
      T * next = entry->logNext;
      delete entry;
      entry = next;
    }
    logFirst = NULL;
    logCount = 0;
  }

  void Push(T * item) {
    item->logNext = NULL;
    
    if (!logCount) {
      logCount = 1;
      logFirst = item;
      return;
    }
    T * entry = logFirst;
    while (entry->logNext) {
      entry = entry->logNext;
    }
    entry->logNext = item;
    logCount++;
  }
  
private:
  int logCount = 0;
  T * logFirst = NULL;
};

}

}

#endif
