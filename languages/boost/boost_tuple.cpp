#include <boost/typeof/typeof.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <iostream>
#include <string>

void testTuple()
{
  boost::tuple<int, int> t4(1, 2);

  BOOST_AUTO(t1, boost::make_tuple(10, "string"));
  std::cout << t1.get<0>() << ":" << t1.get<1>() << std::endl;
  std::cout << t1 << std::endl;

  //std::cout << t1.get<2>();
  //unsigned int j = 0;
  //std::cout << t1.get<j>() << std::endl;

  //for(unsigned int i = 0; i < 2; i++)
  //{
  //  std::cout << t1.get<i>() << std::endl;
  //}

  BOOST_AUTO(t, boost::make_tuple(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));

  BOOST_AUTO(t2, boost::make_tuple(10, "string"));
  //header needed
  bool compare = t1 < t2;

  std::cout << "give a tuple:\n";
  boost::tuple<int, int> t5;
  std::cin >> t5;
  std::cout << t5 << std::endl;
  std::cin >> boost::tuples::set_open('[') >> boost::tuples::set_close(']') >> boost::tuples::set_delimiter(',');
  std::cin >> t5;
  std::cout << boost::tuples::set_open('[') << boost::tuples::set_close(']') << boost::tuples::set_delimiter(',');
  std::cout << t5 << std::endl;
}

boost::tuple<int, int> testTie_f()
{
  return boost::make_tuple(10, 11);
}

void testTie()
{
  int i = 1;
  int j = 2;
  BOOST_AUTO(t1, boost::tie(i, j));

  t1.get<0>() = 3;

  int m = 0;
  int n = 0;
  boost::tie(m, n) = testTie_f();

  int l = 0;
  boost::tie(boost::tuples::ignore, l) = testTie_f();

  //boost::tie(i, j) = boost::tie(j, i);
  boost::tie(i, j) = boost::make_tuple(j, i);
}
