#include <stdlib-api/alloc>
#include <stdlib.h>

extern "C" {

void * AlignMemory(unsigned long size, unsigned long align) {
  void * buf;
  if (posix_memalign(&buf, (size_t)align, (size_t)size)) {
    return NULL;
  }
  return buf;
}

void * AllocMemory(unsigned long size) {
  return malloc((size_t)size);
}

void FreeMemory(void * mem) {
  free(mem);
}

}
