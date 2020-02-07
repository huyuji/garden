#include <iostream>
#include <string>

namespace
{
  // global > class > local
  std::string s("global");

  class A
  {
  public:
    A() : s("class") {};
    void f();

  private:
    std::string s;
  };

  void A::f()
  {
    std::string s("local");
    std::cout << s << std::endl;
  }

  void f1()
  {
    A b;
    b.f();
  }

  // class member of fundamental type are not initialized automatically,
  // including std::nullptr_t
  // this may trigger compiler warning
  class B
  {
  public:
    int i1;
    std::nullptr_t np;
  };

  void f2()
  {
    B b;
    std::cout << b.i1 << std::endl;
    std::cout << (b.np == nullptr) << std::endl;
    std::cout << static_cast<int>(b.np) << std::endl;
  }

  class C {
  public:
    C() { }
    // the following will not suppress the compiler generated copy constructor
    // declare C(C const&, int foo = 0) will 
    C(C const&, int foo);
  }; 

  // make the second parameter have a default argument
  // now this constructor is a copy constructor too. 
  inline C::C(C const&, int foo = 0) {
    std::cout << "weird copy constructor called\n";
  }

  void f3()
  {
    C c1;
    //C c2(c1); // error, more than one constructor
  }

  class D
  {
  public:
    D(int i) 
    {
      std::cout << "ctor of D with one parameter called\n";
    }

    D(int i, int j) 
    { 
      std::cout << "ctor of D with two parameters called\n";
    }

    D(std::initializer_list<int>) 
    {
      std::cout << "ctor of D with initializer list called\n";
    }
  };

  class E
  {
  public:
    E(int, int) 
    {
      std::cout << "ctor of E with two parameters called\n";
    }
  };

  void f4()
  {
    // When there are constructors for both a specific number of arguments and an initializer list, 
    // the version with the initializer list is preferred.
    D d1{ 1 };
    D d2{ 1, 1 };
    D d3(1);
    D d4(1, 1);

    // explicit now also becomes relevant for constructors taking more than one argument.
    D d5 = { 1 };
    D d6 = { 1, 1 };
    E e = { 1, 1 };
  }
}

void testClass()
{
  f4();
}