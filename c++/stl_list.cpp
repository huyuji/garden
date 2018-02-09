#include <list>
#include <iostream>

namespace
{
  using namespace std;

  /*
    at end
    0
    0
  */
  void f1()
  {
    std::list<int> l;
    std::list<int>::iterator it = l.begin();

    l.insert(l.end(), 1);
    if (it == l.end())
    {
      std::cout << "at end\n";
    }
    std::cout << *it << std::endl;
    l.insert(l.end(), 2);
    std::cout << *it << std::endl;
  }

  void f2()
  {
    std::list<int> l = { 2, 1, 4 };
    std::cout << std::distance(l.begin(), l.end());
  }
}

void testStlList()
{
  f2();
}