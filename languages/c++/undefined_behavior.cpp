#include <assert.h>
#include <iostream>

namespace
{
  // variable is modified while being used twice between sequence points
  void f1()
  {
    int i = 19;
    //i = i++; // clang 6.0.0 warning: multiple unsequenced modifications to 'i'
  }
}

void testUndefinedBehavior()
{
  f1();
}