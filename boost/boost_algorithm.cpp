#include <boost/algorithm/string/replace.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>

namespace
{
  void replace()
  {
    std::string s = "\"hello\"world!";
    boost::algorithm::replace_all(s, "\"", "\"\"");
    std::cout << s << std::endl;
  }

  void split()
  {
    std::vector<std::string> objectNames;
    boost::split(objectNames, std::string("a:::b:"), boost::is_any_of(":"));
    std::cout << objectNames.size() << std::endl;

    std::cout << boost::is_any_of(":.")(':');
  }

  void join()
  {
    std::vector<std::string> v;

    using namespace boost::assign;
    push_back(v)("a")("b");

    std::cout << boost::algorithm::join(v, ",");
  }
}

void testAlgorithm()
{
  split();
};