#include <unordered_set>
#include <iostream>
#include <functional>
#include <vector>

namespace
{
  class A
  {
  public:
    A(int key, int value) : key(key), value(value) {}

    int key;
    int value;
  };

  bool operator == (const A& a1, const A& a2)
  {
    return a1.key == a2.key;
  }

  // elements with equivalent keys are adjacent to each other 
  //   when iterating over the elements of the container.
  // Note that the order might not match the order of their insertion.
  // Rehashing and other operations that internally change the order of elements preserve 
  //   the relative order of elements with equivalent keys.
  void f1()
  {
    std::unordered_multiset<A, std::function<int(const A&)>> s(5, [](const A& a) { return a.key; });
    s.insert(A(1, 1));
    s.insert(A(2, 2));
    s.insert(A(1, 2));

    for (const A& a : s)
    {
      std::cout << a.key << " " << a.value << std::endl;
    }
  }

  std::vector<int> u1(const std::unordered_set<int>& s)
  {
    std::vector<int> v;
    for (auto i : s)
    {
      v.push_back(i);
    }

    return v;
  }

  void printVector(const std::vector<int>& v)
  {
    for (unsigned int i = 0; i < v.size(); i++)
    {
      std::cout << v[i] << ((((i + 11) % 10) == 0) ? "\n" : "\t");
    }
    std::cout << "\n\n";
  }

  bool u2(const std::vector<int>& order1, const std::vector<int>& order2, int newValue)
  {
    for (unsigned int i = 0, j = 0; i < order1.size() && j < order2.size(); ++i, ++j)
    {
      if (order2[j] == newValue)
      {
        j++;
      }

      if (order1[i] != order2[j])
      {
        std::cout << "order changed with new value " << newValue << std::endl;
        printVector(order1);
        printVector(order2);
        return false;
      }
    }
  }

  // The order of the elements of unordered container would be undefined.
  // Any insertion can change existing order.In fact, any operation that causes rehashing can change this order.
  void f2()
  {
    std::unordered_set<int> s({ 0 });

    std::vector<int> order = u1(s);
    for (unsigned int i = 1; i < 1024; i++)
    {
      s.insert(i);
      std::vector<int> temp = u1(s);
      u2(order, temp, i);
      order = temp;
    }
  }
}

void testStlUnorderedSet()
{
  f2();
}