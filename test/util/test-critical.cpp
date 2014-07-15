#include "../scoped-pass.hpp"
#include "../assert.hpp"
#include "dummy-api/critical.hpp"
#include <anarch/critical>
#include <cstdlib>

using namespace anarch;
using dummy::SetIgnoreCriticality;

void TestScopedCritical();
void TestAssertCriticality();

int main() {
  TestScopedCritical();
  TestAssertCriticality();
  return 0;
}

void TestScopedCritical() {
  ScopedPass pass("ScopedCritical");
  
  SetIgnoreCriticality(false);
  SetCritical(false);
  
  Assert(!GetCritical());
  {
    ScopedCritical critical;
    Assert(GetCritical());
  }
  Assert(!GetCritical());
}

void TestAssertCriticality() {
  ScopedPass pass("[AssertNoncritical/AssertCritical]()");
  
  SetIgnoreCriticality(true);
  try {
    SetCritical(true);
    AssertCritical();
    AssertNoncritical();
    SetCritical(false);
    AssertCritical();
    AssertNoncritical();
  } catch (const char * exc) {
    std::cerr << "unexpected panic: " << exc << std::endl;
    exit(1);
  }
  
  SetIgnoreCriticality(false);
  SetCritical(true);
  try {
    AssertCritical();
  } catch (const char * exc) {
    std::cerr << "unexpected panic: " << exc << std::endl;
    exit(1);
  }
  try {
    AssertNoncritical();
    std::cerr << "expected panic after AssertNoncritical()" << std::endl;
    exit(1);
  } catch (const char * exc) {
  }
  
  SetCritical(false);
  try {
    AssertNoncritical();
  } catch (const char * exc) {
    std::cerr << "unexpected panic: " << exc << std::endl;
    exit(1);
  }
  try {
    AssertCritical();
    std::cerr << "expected panic after AssertCritical()" << std::endl;
    exit(1);
  } catch (const char * exc) {
  }
}
