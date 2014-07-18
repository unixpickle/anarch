#include <stdlib-api/print>
#include <stdio.h>

extern "C" {

void PrintString(const char * str) {
  printf("%s", str);
  fflush(stdout);
}

void PrintError(const char * str) {
  fprintf(stderr, "%s", str);
  fflush(stderr);
}

void PrintNumber(unsigned long long number) {
  printf("0x%llx", number);
  fflush(stdout);
}

}
