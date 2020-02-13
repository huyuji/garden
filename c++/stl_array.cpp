#include <array>
#include <iostream>
#include <assert.h>

namespace
{
  void f1()
  {
    // Because no constructors or assignment operators for initializer lists are provided, 
    //   initializing an array during its declaration is the only way to use initializer lists. 
    // For this reason, you also can��t use the parenthesis syntax to specify initial values
    //std::array<int, 5> a({ 1, 2, 3, 4, 5 });

    std::array<int, 5> a = { 1, 2, 3, 4, 5 };
    // Class array<> being an aggregate also means that the member that holds all the elements is public. 
    // However, its name is not specified in the standard; 
    //   thus, any direct access to the public member that holds all elements 
    //   results in undefined behavior and is definitely not portable.
    //std::cout << a._Elems << std::endl; // works with visual studio
  }

  void f2()
  {
    // It is possible to specify a size of 0, which is an array with no elements. 
    std::array<int, 0> a;

    // However, the return value of front()  and back()  is undefined
    // the following code will print random value (release build) or crash the program (debug build)
    int i = a.front();
    int j = a.back();
    std::cout << i << "," << j << std::endl;

    // In that case, begin()  and end() , cbegin()  and cend() , 
    //   and the corresponding reverse iterators still yield the same unique value. 
    assert(a.begin() == a.end());
    assert(a.cbegin() == a.cend());
    
    // For data() , the return value is unspecified, 
    //   which means that you can pass the return value to other places as long as you don��t dereference it.
    int* pi = a.data();
  }

  // Arrays provide the tuple interface
  void f3()
  {
    const int size = 3;
    std::array<int, size> a;
    std::tuple_element<2, decltype(a)>::type i; //int
    assert(size == std::tuple_size < decltype(a) >::value);
  }

  // swap() of array has linear complexity, 
  // and iterators and references refer to the same container but different elements afterward.
  void f4()
  {
    std::array<int, 2> a1 = { 1, 2 };
    auto it1 = a1.begin();
    int& ref1 = a1[0];
    std::array<int, 2> a2 = { 3, 4 };
    auto it2 = a2.begin();
    int& ref2 = a2[0];

    std::swap(a1, a2);
    std::cout << *it1 << " " << *it2 << std::endl;
    std::cout << ref1 << " " << ref2 << std::endl;
  }
}

void testStlArray()
{
  f4();
}