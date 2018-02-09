#include "utility.h"
#include <stack>
#include <boost/assign.hpp>

namespace
{
  void sort_stack(std::stack<int>& unsorted)
  {
    std::stack<int> sorted;

    while (unsorted.size())
    {
      int current = unsorted.top();
      unsorted.pop();

      int count = 0;

      while (sorted.size() && current > sorted.top())
      {
        unsorted.push(sorted.top());
        sorted.pop();
        count++;
      }

      sorted.push(current);

      while (count--)
      {
        sorted.push(unsorted.top());
        unsorted.pop();
      }
    }

    while (sorted.size())
    {
      unsorted.push(sorted.top());
      sorted.pop();
    }
  }
}

void test_sort_stack()
{
  using namespace boost::assign;
  std::stack<int> s;
  s.push(5);
  s.push(4);
  s.push(3);
  s.push(2);
  s.push(1);
  sort_stack(s);
  while(s.size())
  {
    std::cout << s.top() << std::endl;
    s.pop();
  }
}