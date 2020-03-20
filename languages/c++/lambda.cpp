#include <iostream>
#include <typeinfo>
#include <functional>
#include <array>

namespace
{
  void f1()
  {
    // return type of lambda is deduced from return value if not specified
    auto l1 = [] { return 11; };
    std::cout << l1() << std::endl;
  }

  void f2()
  {
    int i = 13;
    //[=] { i++; }(); // a lambda would not compile if it attempts to change a capture copied by value

    // with mutable, capture copied by value can be modified
    // and the lambda becomes stateful since the modification will be visible in subsequent calls
    auto l = [=]() mutable { i++; std::cout << "in lambda i is " << i << std::endl; };
    l();
    l();
    std::cout << "outside lambda i is " << i << std::endl;
  }

  // The type of a lambda is an anonymous function object (or functor) that is unique for each lambda expression.
  // auto, decltype and function template can deal with the type of lambda
  void f3()
  {
    auto l1 = [] {};
    auto l2 = [] {};
    std::cout << bool(typeid(l1) == typeid(l2)) << std::endl;

    std::function<void()> f1 = l1;
    std::function<void()> f2 = l2;
    std::cout << bool(typeid(f1) == typeid(f2)) << std::endl;
  }

  void f4()
  {
    // lambdas should have no default constructor.
    // but it seems it works in visual studio.
	// it seems this has been fixed in visual studio 2017.
	auto l1 = []{ std::cout << "l1 called\n"; };
	// std::array<decltype(l1), 3> a1;
    // a1[2]();

    auto l3 = []{ std::cout << "l2 called\n"; };
    // lambdas have no assignment operator. 
    // this will not compile.
    //l3 = l1;
  }

  void generic_lambda()
  {
    auto gl = [] (auto a) { return a; };
    std::cout << gl(1) << '\n' << gl('a') << '\n';
  }
}

void testLambda()
{
  generic_lambda();
}