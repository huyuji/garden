#include <iostream>

namespace
{
  // both const <type> * and <type> const * mean what's pointed to is constant
  void f1()
  {
    char c = 'z';
    char c1 = 'y';
    char * p = &c1;

    const char c2 = c;
    //c2 = c1;

    char const c3 = c;
    //c3 = c1;

    // duplicate const qualifer may trigger warning but is not an error
    const const char const const c5 = c;

    const char * p1 = &c;
    //*p1 = c1;
    p1 = p;

    char const * p2 = &c;
    //*p2 = c1;
    p2 = p;

    char * const p3 = &c;
    *p3 = c1;
    //p3 = p;

    const char const * p4 = &c;
    //*p4 = c1;
    p4 = p;

    const char * const p5 = &c;
    //*p5 = c1;
    //p5 = p;

    char const * const p6 = &c;
    //*p6 = c1;
    //p6 = p;

    const char const * const p7 = &c;
    //*p7 = c1;
    //p7 = p;
  }

  // a const member function isn't allowed to modify 
  // any of the non-static data members of the object on which it is invoked
  class A
  {
  public:
    void f() const
    {
      i = 10;
      //j = 11;
    };

  private:
    static int i;
    int j;

  };

  int A::i;

  void f2()
  {
    A c1;
    c1.f();
  }
}

void testConst()
{
  f2();
}