#ifndef __ANARCH_UTIL_NEW_HPP__
#define __ANARCH_UTIL_NEW_HPP__

#include <anarch/stddef>

inline void * operator new(size_t, void * p) {
  return p;
}

inline void * operator new[](size_t, void * p) {
  return p;
}

inline void operator delete(void *, void *) {
}

inline void operator delete[](void *, void *) {
}

#endif
