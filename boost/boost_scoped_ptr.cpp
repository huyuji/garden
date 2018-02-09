#include <boost\scoped_ptr.hpp>
#include <iostream>

void testScopedPtr()
{
  boost::scoped_ptr<int> p;
  if(!p)
  {
    std::cout << "empty pointer\n";
  }
}