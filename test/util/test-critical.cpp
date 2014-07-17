#include <stdlib-api/assert>
#include <stdlib-api/scoped-pass>
#include <stdlib-api/print>

#include "dummy-api/critical.hpp"
#include <anarch/critical>

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
    PrintError("unexpected panic: ");
    Die(exc);
  }
  
  SetIgnoreCriticality(false);
  SetCritical(true);
  try {
    AssertCritical();
  } catch (const char * exc) {
    PrintError("unexpected panic: ");
    Die(exc);
  }
  try {
    AssertNoncritical();
    Die("expected panic after AssertNoncritical()");
  } catch (const char * exc) {
  }
  
  SetCritical(false);
  try {
    AssertNoncritical();
  } catch (const char * exc) {
    PrintError("unexpected panic: ");
    Die(exc);
  }
  try {
    AssertCritical();
    Die("expected panic after AssertCritical()");
  } catch (const char * exc) {
  }
}
