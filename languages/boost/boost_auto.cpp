#include <utility>
#include <boost/typeof/typeof.hpp>
#include <iostream>

struct boost_auto_a 
{
  int i; boost_auto_a(): i(10) 
  { std::cout << "boost_auto_a ctor\n"; }; 
  boost_auto_a(const boost_auto_a&) {std::cout << "boost_auto_a copy ctor\n";  };
  ~boost_auto_a() { std::cout << "boost_auto_a dtor\n"; };
};

//typeof and auto
//#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()
BOOST_TYPEOF_REGISTER_TYPE(boost_auto_a)

void testAuto()
{
  BOOST_AUTO(p, std::make_pair(1, "string"));
  std::cout << p.first << ":" << p.second << std::endl;

  BOOST_AUTO(q, boost_auto_a());
  std::cout << q.i << std::endl;
}