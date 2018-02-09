#include <iostream>

#define M1(A) { std::cout << A << std::endl; }
#define M2 M1

void testMacro()
{
  M2(3);
}