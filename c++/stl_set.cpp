#include <set>
#include <iostream>
#include <functional>
#include <vector>

namespace
{
  void f1()
  {
    class less
    {
    public:
      int id;

      less(int i) : id(i) {}
      bool operator()(int i, int j){ return i < j; }
    };

    std::set<int, less> s1({ 1, 2, 3 }, less(1));
    std::set<int, less> s2({ 4, 5, 6 }, less(2));

    // the assignment operator (of set or multiset) assigns the elements and the sorting criterion; 
    // otherwise, an assignment would be an easy way to compromise the sorting criterion.
    s1 = s2;
    std::cout << s1.comp.id;
  }

  // set of iterators
  void f2()
  {
    std::vector<int> v = { 1, 4, 3, 5, 2 };
    std::set<std::vector<int>::iterator> s;
    for (auto it = v.begin(); it != v.end(); ++it)
    {
      s.insert(it);
    }

    std::cout << typeid(*s.begin()).name() << std::endl;

    for (auto it : s)
    {
      std::cout << *it << " ";
    }
    std::cout << std::endl;
  }

  class A
  {
  public:
    A(int key, int value) : key(key), value(value) {}

    int key;
    int value;
  };

  bool operator < (const A& a1, const A& a2)
  {
    return a1.key < a2.key;
  }

  // For multisets, the order of equivalent elements is random but stable. 
  // Thus, insertions and erasures preserve the relative ordering of equivalent elements (guaranteed since C++11).
  void f3()
  {
    std::multiset<A> s({ { 1, 1 }, { 2, 2 }, { 1, 3 }, { 3, 1 } });
    s.emplace(A{ 1, 2 });
    for (const auto& item : s)
    {
      std::cout << item.key << "-" << item.value << std::endl;
    }
  }

  void f4()
  {
    std::set<int> s1;
    std::set<int, std::function<bool(int, int)>> s2([](int i, int j){ return i < j; });
    std::set<long> s3;

    // For assignment of set or multiset, both containers must have the same type. 
    // In particular, the type of the comparison criteria must be the same, 
    //   although the comparison criteria themselves may be different. 
    //s1 = s2;
    //s1 = s3;
  }
}

void testStlSet()
{
  f3();
}