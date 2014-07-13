#ifndef __ANARCH_API_PANIC_HPP__
#define __ANARCH_API_PANIC_HPP__

#include <ansa/macros>

namespace anarch {

void Panic(const char * msg) ANSA_NORETURN;

}

#endif
