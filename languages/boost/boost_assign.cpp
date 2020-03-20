#include <boost/assign.hpp>
#include <stdlib.h>
#include <map>
#include <iostream>
#include <string>

void testAssign()
{
  //must be declared
  using namespace boost::assign;

  //operator
  std::vector<int> v1;
  v1 += 1,2,3;
  
  std::map<int, std::string> m1;
  m1 += std::make_pair(1, "one"), std::make_pair(2, "two");

  //utility function
  std::vector<int> v2;
  push_back(v2)(1)(2)(3);

  std::list<int> l2;
  push_front(l2)(1)(2)(3);

  std::map<int, std::string> m2;
  insert(m2)(1, "one")(2, "two");

  //mixing
  std::vector<int> v3;
  push_back(v3),1,2,3;
  
  push_back(v3)() = 4,5,6;

  //initializing
  //list_of
  std::vector<int> v4 = list_of(1)(2)(3);

  std::set<int> s4 = (list_of(1), 2, 3);

  std::map<int, std::string> m4 = list_of(std::make_pair(1, "one"))(std::make_pair(2, "two"));

  //map_list_of/pair_list_of
  std::map<int, std::string> m5 = map_list_of(1, "one")(2, "two");

  typedef boost::tuple<int, int, int> Tuple;
  std::vector<Tuple> v = tuple_list_of(1, 1, 1)(2, 2, 2);

  std::vector<int> v6 = list_of(1).repeat(1, 2)(3);

  std::vector<int> v7;
  push_back(v7).repeat_fun(5, &rand).repeat(2, 1), 10;

  std::vector<int> v8;
  push_back(v8).range(v7.begin(), v7.begin() + 3);

  std::vector<std::vector<int> > v9 = list_of(list_of(1)(2)(3))(list_of(4)(5)(6));
}
