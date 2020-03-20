#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>
#include <sstream>

void test()
{
    while(1)
  {
    std::string s;
    std::cin >> s;
    double d = boost::lexical_cast<double>(s);
    std::cout << boost::lexical_cast<std::string>((unsigned long long)(d + 0.5)) << std::endl;
  }

  //try
  //{
  //  std::cout << boost::lexical_cast<int>("a") << std::endl;
  //}
  //catch (boost::bad_lexical_cast& e)
  //{
  //  std::cout << e.what() << std::endl;
  //}
}

void test1()
{
  std::stringstream ss;
  ss << "<:inf:>0.000000";
  double n = 1;
  ss >> n;
  try
  {
    std::cout << boost::lexical_cast<double>("<:inf:>0.000000");
  }
  catch(boost::bad_lexical_cast& e)
  {
    std::cout << e.what();
  }
}

void testLexicalCast()
{
  test1();
}
