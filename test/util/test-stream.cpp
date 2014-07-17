#include <stdlib-api/scoped-pass>
#include <stdlib-api/assert>
#include <anarch/stream>
#include <ansa/cstring>
#include "dummy-api/console.hpp"

using namespace anarch;

int main() {
  ScopedPass pass("[cout/cerr/StreamModule]");
  
  StreamModule::GetGlobal().Load();
  cout << "abcd" << endl;
  cerr << "efgh" << endl;
  cout << (unsigned int)32 << endl;
  
  dummy::LogConsole & console = dummy::LogConsole::GetGlobal();
  Assert(console.GetBacklogSize() == 15);
  Assert(ansa::Memcmp(console.GetBacklog(), "abcd\nefgh\n0x20\n", 15) == 0);
  
  return 0;
}
