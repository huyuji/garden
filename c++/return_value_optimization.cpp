#include <iostream>

namespace
{
  class B
  {
  public:
    B() { std::cout << "ctor of B called\n"; }

    B(const B&) { std::cout << "copy ctor of B called\n"; }

    B& operator = (const B&) { std::cout << "copy assignment operator of B called\n"; return *this; }

    ~B() { std::cout << "dtor of B called\n"; }
  };

  B u1() { B b; return b; }

  /*
  Debug build of vs2013, optimization disabled
    ctor of B called
    ctor of B called
    copy ctor of B called
    dtor of B called
    copy assignment operator of B called
    dtor of B called
    dtor of B called
  Release build of vs2013, full optimization
    ctor of B called
    ctor of B called
    copy assignment operator of B called
    dtor of B called
    dtor of B called
  */
  void f1() { B b; b = u1(); }

  /*
  Debug build of vs2013, optimization disabled
    ctor of B called
    copy ctor of B called
    dtor of B called
    dtor of B called
  Release build of vs2013, full optimization
    ctor of B called
    dtor of B called
  */
  void f2() { B b = u1(); }

  class C
  {
  public:
    C() { std::cout << "ctor of C called\n"; }

    C(const C&) { std::cout << "copy ctor of C called\n"; }

    C(const C&&) { std::cout << "move ctor of C called\n"; }

    C& operator = (const C&) { std::cout << "copy assignment operator of C called\n"; return *this; }

    ~C() { std::cout << "dtor of C called\n"; }
  };

  C u2() { C c; return c; }

  /*
  Debug build of vs2013, optimization disabled
    ctor of C called
    ctor of C called
    move ctor of C called
    dtor of C called
    copy assignment operator of C called
    dtor of C called
    dtor of C called
  Release build of vs2013, full optimization
    ctor of C called
    ctor of C called
    copy assignment operator of C called
    dtor of C called
    dtor of C called
  */
  void f3() { C c; c = u2(); }

  /*
  Debug build of vs2013, optimization disabled
    ctor of C called
    move ctor of C called
    dtor of C called
    dtor of C called
  Release build of vs2013, full optimization
    ctor of C called
    dtor of C called
  */
  void f4() { C c = u2(); }
}

void testReturnValueOptimization()
{
  f3();
}