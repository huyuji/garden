#include <assert.h>
#include <iostream>

namespace
{
  void u1(int ai[3][4])
  {
    std::cout << sizeof(ai) << std::endl;
  }

  void u2(int(&ai)[3][4])
  {
    std::cout << sizeof(ai) << std::endl;
  }

  struct A
  {
    int ai[3][4];
  };

  void f1()
  {
    int ai[3][4];
    std::cout << sizeof(ai) << std::endl; // 48
    u1(ai);                               // 4
    u2(ai);                               // 48
    A* pa = new A();
    std::cout << sizeof(pa->ai) << std::endl; // 48
  }

  int u3(int& i)
  {
    return i++;
  }

  // if the parameter of sizeof is an expression, the expression is never evaluated
  void f2()
  {
    int i = 13;
    std::cout << sizeof(i++) << " " << i << std::endl;
    std::cout << sizeof(u3(i)) << " " << i << std::endl;
  }

  void f3()
  {
  }
}

void testSizeof()
{
  f1();
}