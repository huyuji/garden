#include <iostream>
#include <ctime>
#include <vector>
#include <deque>
#include <iterator>
#include <memory>
#include <assert.h>
#include <limits>
#include <string.h>

namespace
{
  int f1()
  {
    return 10;
  }

  //It's never legal to modify the return value of a function that returns a built-in type.
  void f2()
  {
    //std::cout << (f2() = 11);
  }

  void f3()
  {
    typedef short TV;
    std::cout << std::numeric_limits<TV>::is_integer;
  }

  void f4()
  {
    time_t t = 0x53898C2C;
    std::cout << asctime(gmtime(&t)) << std::endl;
  }

  // initializer list forces value initialization
  void f5()
  {
    int i1;
    int * p1;
    int i2{};
    int * p2{};

    // uninitialized
    std::cout << i1 << std::endl;
    std::cout << p1 << std::endl;

    // initialized
    std::cout << i2 << std::endl;
    std::cout << p2 << std::endl;
  }

  // initializer list doesn't allow narrowing initialization, not even 5.0 to 5
  void f6()
  {
    int x1(5.3);  // OK, but OUCH: x1 becomes 5
    int x2 = 5.3;  // OK, but OUCH: x2 becomes 5
    //int x3{ 5.0 };  // ERROR: narrowing
    //int x4 = { 5.3 };  // ERROR: narrowing
    float x5{ 5.0 };
  }

  class A
  {
  public:
    explicit A(int i) {};
  };

  void f7()
  {
    std::vector<int> v = { 1, 2 };
    // no explicit type conversions are possible when elements are initialized inside the for loop
    // Implicit conversion still works.
    //for (A a : v) { }
  }

  void f8()
  {
    {
      // read all integer elements of the deque from standard input
      std::deque<int> c{ std::istream_iterator<int>(std::cin), std::istream_iterator<int>() };
    }

    {
      // read all integer elements of the deque from standard input
      std::deque<int> c((std::istream_iterator<int>(std::cin)), (std::istream_iterator<int>()));
    }

    // In this case, c declares a function having deque<int> as return type. 
    // Its first parameter is of type istream_iterator<int> with the name cin, 
    //   and its second unnamed parameter is of type ��function taking no arguments returning istream_iterator<int>.�� 
    // This construct is valid syntactically as either a declaration or an expression. 
    // So, according to language rules, it is treated as a declaration. 
    // The extra parentheses in the line above force the initializer not to match the syntax of a declaration.
    //std::deque<int> c(std::istream_iterator<int>(std::cin), std::istream_iterator<int>());
  }

  void f9()
  {
    int ai[12];
    memset(ai, 0, 12); // 12 is incorrect, should be 12*sizeof(int)
  }

  class B
  {
  public:
    int i;
  };

  void f10()
  {
    std::shared_ptr<B> sp(new B[12]); // error, B[12] will not be fully deleted
    sp->i = 17;
  }

  void f11()
  {
    unsigned char uc = -1;
    assert(uc != -1); // both arguments will be lifted to int
    signed char c = 128;
    assert(c != 128); // signed char is ranged from -128 to 127

    size_t size = 12;
    assert(size - 13 > 0); // unsigned type arithmic produces unsigned type which is always greater than zero
  }
}

void testCpp()
{
  f11();
}