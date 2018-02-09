#include <iostream>

namespace
{
  void f1()
  {
    // the type declared by the catch clause must be an exact match to catch exception of fundamental type
    try
    {
      throw 20;
    }
    catch (int e)
    {
      std::cout << "An exception occurred. Exception Nr. " << e << std::endl;
    }

    try
    {
      throw 20U;
    }
    catch (unsigned int e)
    {
      std::cout << "An exception occurred. Exception Nr. " << e << std::endl;
    }
  }
}

void testException()
{
  f1();
}