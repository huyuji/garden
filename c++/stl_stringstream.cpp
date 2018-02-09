#include <sstream>
#include <iostream>

namespace
{
  void f1()
  {
    std::stringstream ss;
    ss << "1 2";

    unsigned int i, j, k;
    ss >> i >> j >> k;
    // k will be random
    std::cout << i << "-" << j << "-" << k << std::endl;
  }

  void f2()
  {
    std::stringstream ss;
    ss << "   \"title\"";
    char tmpChar;
    ss >> tmpChar;
    std::string tmpStr;
    std::getline(ss, tmpStr, '"');
    std::cout << tmpChar << std::endl // "
      << tmpStr << std::endl;
  }
}

void testStlStringStream()
{
  f2();
}