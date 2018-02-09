#include <iostream>

namespace
{
  inline int* f1()
  {
    static int i = 10;
    int j = 11;
    return &j;
  }

  void f2()
  {
    int* i = f1();
    std::cout << i << std::endl;
  }
}

void testInline()
{
  f2();
}