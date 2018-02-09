#include <boost/foreach.hpp>
#include <boost/assign.hpp>
#include <iostream>
#include <string>

void testForeach()
{
  std::string s("string");
  BOOST_FOREACH(char c, s)
  {
    std::cout << c << " ";
  }
}
