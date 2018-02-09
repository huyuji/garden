#include <iostream>

namespace
{
  // usually, C++ requires that you provide a definition for anything you use, 
  // but class-specific constants that are static and of integral type 
  // (e.g., integers, chars, and bools) are exceptions. 
  // As long as you don't take their address (yes for gcc, no for vc2010), 
  // you can declare them and use them without providing a definition.
  class A
  {
  public:
    static const int i = 10;
    //static int j = 10;
    static const bool b = true;
    static const char c = 'z';
  };

  void f1()
  {
    std::cout << A::i << std::endl
      << A::b << std::endl
      << A::c << std::endl
      << &A::i << std::endl
      ;
  }
}

void testStatic()
{
  f1();
}