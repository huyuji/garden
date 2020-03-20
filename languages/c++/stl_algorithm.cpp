#include <algorithm>
#include <iostream>
#include <assert.h>
#include <list>

namespace
{
  class A
  {
  public:
    int id;

    A(int i): id(i) { std::cout << "ctor of A\n"; }
    ~A() { std::cout << "dtor of A\n"; }
    A(const A& a) { id = a.id;  std::cout << "copy ctor of A\n"; }
    A& operator=(const A& a){ id = a.id; std::cout << "assignment operator of A\n"; return *this; }
  };

  bool operator==(const A& a1, const A& a2)
  {
    return a1.id == a2.id;
  }

  // algorithm remove "removes" elements by overwriting insteading of erasing.
  void f1()
  {
    std::list<A> l = {1,2,3};
    std::cout << "remove 1\n";
    std::remove(l.begin(), l.end(), 1);
  }
}

void testStlAlgorithm()
{
  f1();
}