#include <iostream>
#include <string>

namespace
{
  void f1()
  {
    std::string str1 = "45";
    std::string str2 = "3.14159";
    std::string str3 = "31337 with words";
    std::string str4 = "words and 2";

    int myint1 = std::stoi(str1);
    int myint2 = std::stoi(str2);
    int myint3 = std::stoi(str3);
    // error: 'std::invalid_argument'
    // int myint4 = std::stoi(str4);

    std::cout << "std::stoi(\"" << str1 << "\") is " << myint1 << '\n';
    std::cout << "std::stoi(\"" << str2 << "\") is " << myint2 << '\n';
    std::cout << "std::stoi(\"" << str3 << "\") is " << myint3 << '\n';
    //std::cout << "std::stoi(\"" << str4 << "\") is " << myint4 << '\n';
  }

  // reserve() has no effect on the string length and cannot alter its content.
  // reserve a capacity smaller than the current may or may not reduce the capacity.
  void f2()
  {
    std::string s("12345");
    std::cout << s.capacity() << std::endl;
    s.reserve(1024);
    std::cout << s.capacity() << std::endl;
    s.reserve(128);
    std::cout << s.capacity() << std::endl;
  }

  void f3()
  {

  }
}

void testStlString()
{
  f2();
}