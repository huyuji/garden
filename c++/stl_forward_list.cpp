#include <forward_list>
#include <iostream>

namespace
{
  void f1()
  {
    std::forward_list<int> fl = { 2, 5, 3 };
    std::cout << std::distance(fl.before_begin(), fl.end());
  }
}

void testStlForwardList()
{
  f1();
}