#include <tuple>
#include <iostream>
#include <assert.h>

namespace
{
  using namespace std;
  void f1()
  {
    auto cpi = make_tuple(4, 0.1, -0.2);
    tuple_element <0, decltype(cpi)>::type val1;//int
    tuple_element <1, decltype(cpi)>::type val2;//double
    tuple_element <2, decltype(cpi)>::type val3;//double
  }
}

void testStlTuple()
{
  f1();
}