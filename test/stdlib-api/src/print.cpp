#include <stdlib-api/print>
#include <stdio.h>

extern "C" {

void PrintString(const char * str) {
  printf("%s", str);
  fflush(stdout);
}

}
