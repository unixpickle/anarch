#include <anarch/stream>

namespace anarch {

namespace x64 {

void InitializeSingletons(void * mbootPtr) {
  StreamModule::InitGlobal();
  // TODO: here, initialize singletons and pass around the multiboot pointer
}

}

}
