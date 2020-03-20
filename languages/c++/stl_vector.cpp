#include <vector>
#include <iostream>
#include <string>
#include <assert.h>

namespace
{
  class A
  {
  public:
    A() { std::cout << "ctor of A\n"; };
    A(A&) { std::cout << "copy ctor of A\n"; };
    ~A() { std::cout << "dtor of A\n"; };
  };

  /*
    ctor of A
    ctor of A
    2
    ctor of A
    copy ctor of A
    copy ctor of A
    dtor of A
    dtor of A
    copy ctor of A
    dtor of A
    dtor of A
    dtor of A
    dtor of A
  */
  void f1()
  {
    std::vector<A> v(2);
    std::cout << v.capacity() << std::endl;
  }

  void f2()
  {
    std::vector<int> v;
    for (int i = -3; i < 7; i++)
    {
      v.push_back(i);
    }

    std::vector<int>::iterator it = v.end();
    // the follow statement might return successfully because vector has reserved space
    std::cout << it[3];
  }

  void f3()
  {
    std::vector<int> v = { 1, 2, 3 };
    // As for class vector<>, 
    // the C++ standard library guarantees that the elements of an array<> are in contiguous memory. 
    // Thus, you can expect that for any valid index i in array a, the following yields true: 
    assert(&v[2] == &v[1] + 1);
  }

  void f4()
  {
    std::vector<int> v = { 1, 2, 3 };
    auto it = v.begin();
    v.resize(32);
    // Reallocation invalidates all references, pointers, and iterators for elements of the vector.
    // The following line will print a random value (in Release build)
    std::cout << *it << std::endl;
  }

  // reserve() has no effect on the vector size and cannot alter its elements.
  // the capacity of vectors never shrinks.
  // reserve a capacity smaller than the current has no effect.
  void f5()
  {
    std::vector<int> v = { 1, 2, 3 };
    std::cout << v.capacity() << std::endl;
    v.reserve(1024);
    std::cout << v.capacity() << std::endl;
    v.reserve(16);
    std::cout << v.capacity() << std::endl;
  }

  // shrink_to_fit() is nonbinding
  void f6()
  {
    std::vector<int> v;
    for (unsigned int i = 0; i < 4096; i++)
    {
      v.push_back(i);
    }
    std::cout << v.capacity() << std::endl;
    v.shrink_to_fit();
    std::cout << v.capacity() << std::endl;
  }

  // equivalent of shrink_to_fit before c++11
  template <typename T>
  void shrinkCapacity(std::vector<T>& v)
  {
    std::vector<T> tmp(v);  // copy elements into a new vector
    v.swap(tmp);  // swap internal vector data
  }

  // only at() performs range checking.
  void f7()
  {
    std::vector<int> v = { 1, 3, 2 };
    try
    {
      std::cout << v.at(4);
    }
    catch (std::out_of_range& e)
    {
      std::cout << e.what() << std::endl;
    }
  }

  // Calling operator [] , front() , and back(), pop_back()  for an empty container
  //   always results in undefined behavior.
  // In the following program, pop_back() has no effect, other 3 operations crashes the program.
  void f8()
  {
    std::vector<int> v;
    std::cout << v[0] << std::endl;
    std::cout << v.front() << std::endl;
    std::cout << v.back() << std::endl;
    v.pop_back();
  }
}

void testStlVector()
{
  f8();
}
