#include <vector>
#include <iostream>
#include <tuple>
#include <map>
#include <set>

namespace
{
  // STL containers where the value was moved from are guaranteed to be empty
  void f1()
  {
    std::vector<int> v1 = {1,2,3};
    std::vector<int> v2 = std::move(v1);
    std::cout << v1.size() << std::endl;
  }

  // reference_wrapper
  void f2()
  {
    int i = 10, j = 11;
    std::vector<std::reference_wrapper<int>> v = { i, j };
    for(int& it : v)
    {
      std::cout << it << " ";
    }
    std::cout << std::endl;

    i *= 10;
    j *= 10;

    for (int& it : v)
    {
      std::cout << it << " ";
    }
    std::cout << std::endl;
  }

  // In order to avoid compromising the internal order, 
  //   every iterator for an associative and unordered container is declared as 
  //   an iterator for a constant value or key.
  // However, reference_wrapper can violate this.
  void f3()
  {
    int i = 10, j = 11;
    std::set<std::reference_wrapper<int>> s({ i, j });
    for (auto it : s)
    {
      std::cout << it << " ";
    }
    std::cout << std::endl;

    i = 11;
    j = 10;
    for (auto it : s)
    {
      std::cout << it << " ";
    }
    std::cout << std::endl;
  }
}

void testStl()
{
  f3();
}