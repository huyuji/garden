#include <map>
#include <iostream>
#include <functional>
#include <string>

namespace
{
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

  // For multimaps, the order of elements with equivalent keys is random but stable. 
  // Thus, insertions and erasures preserve the relative ordering of equivalent elements (guaranteed since C++11)
  void f1()
  {
    std::multimap<A, int> m({ 
        { { 1, 1 }, 1 }, 
        { { 2, 1 }, 2 }, 
        { { 1, 2 }, 3 } 
    });

    for (auto const & item : m)
    {
      std::cout << "(" << item.first.key << "," << item.first.value << ")->" << item.second << std::endl;
    }
    std::cout << std::endl;
  }

  void f2()
  {
    std::map<int, int> m1;
    std::map<int, float> m2;
    std::map<int, int, std::function<bool(int, int)>> m3([](int i, int j){ return i < j; });
    
    // Comparisons of map/multimap are provided only for containers of the same type. 
    // Thus, the key, the value, and the sorting criterion must be of the same type.
    //m1 == m2;
    //m1 == m3;
  }

  void f3()
  {
    std::map<int, int> m({ { 1, 1 }, { 2, 2 } });
  
    std::pair<const int, int> item1 = *m.begin();
    
    std::map<int, int>::value_type item2 = *m.begin();
    
    typedef decltype(m) Element;
    Element::value_type item3 = *m.begin();
    
    // gcc supports it better
    //decltype(m)::value_type item3 = *m.begin();
  }

  // at() yields the value of the element with the passed key and 
  //   throws an exception object of type out_of_range if no such element is present.
  void f4()
  {
    std::map<int, int> m({ { 1, 1 }, { 2, 2 } });

    try
    {
      std::cout << m.at(3);
    }
    catch (std::out_of_range& e)
    {
      std::cout << e.what() << std::endl;
    }
  }

  // For operator [], the type of the index is not the only difference from ordinary arrays. 
  // In addition, you can¡¯t have a wrong index. 
  // If you use a key as the index for which no element yet exists, 
  //   a new element gets inserted into the map automatically. 
  // The value of the new element is initialized by the default constructor of its type. 
  // Thus, to use this feature, you can¡¯t use a value type that has no default constructor.
  void f5()
  {
    class A
    {
    public: 
      A(int){}
    };

    std::map<int, A> m;
    // 
    //A a = m[0];
  }

  void f6()
  {
    std::map<std::string, std::string> m;

    // uses pair's move constructor
    m.emplace(std::make_pair(std::string("a"), std::string("a")));

    // uses pair's converting move constructor
    m.emplace(std::make_pair("b", "abcd"));

    // uses pair's template constructor
    m.emplace("d", "ddd");

    // uses pair's piecewise constructor
    m.emplace(std::piecewise_construct,
      std::forward_as_tuple("c"),
      std::forward_as_tuple(10, 'c'));

    for (const auto &p : m) {
      std::cout << p.first << " => " << p.second << '\n';
    }
  }
}

void testStlMap()
{
  f6();
}