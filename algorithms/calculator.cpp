#include "utility.h"
#include <fstream>
#include <sstream>
#include "boost/algorithm/string.hpp"
#include <vector>

namespace
{
  int calculate(const char *c)
  {
    std::vector<std::string> sums;
    boost::algorithm::split(sums, std::string(c), boost::is_any_of("+"));

    unsigned int sum = 0;
    for (unsigned int i = 0; i < sums.size(); i++)
    {
      std::vector<std::string> factors;
      boost::algorithm::split(factors, sums[i], boost::is_any_of("*"));

      unsigned int product = 1;
      for (unsigned int j = 0; j < factors.size(); j++)
      {
        product *= atoi(factors[j].c_str());
      }

      sum += product;
    }

    return sum;
  }
}

void test_calculate()
{
  char buffer[256];

  std::ifstream file("c:\\volatile\\code\\temp\\codeblocks\\bin\\Debug\\test_calculate");
  while(!file.eof())
  {
    file.getline(buffer, 256);
    //std::cout << "process " << buffer << std::endl;
    if(calculate(buffer) != 1808)
    {
      std::cout << "wrong answer: " << buffer << std::endl;
    }
  }
}
