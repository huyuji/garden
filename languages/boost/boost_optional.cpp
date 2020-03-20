#include <vector>
#include <boost/optional.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <iostream>
#include <string>

void testOptional()
{
  boost::optional<std::string> p;
  if(!p) { std::cout << "not initialized\n"; }
  //std::cout << *p << std::endl;

  std::cout << p.get_value_or("default string") << std::endl;

  p.reset("some string");
  std::cout << p.get_value_or("default string") << std::endl;

  std::vector<int> v;
  boost::optional<std::vector<int> > q(v);
  boost::optional<std::vector<int>& > r(v);
  q->push_back(1);
  std::cout << v.size() << " " << q->size() << " " << r->size() << std::endl;

  boost::optional<std::vector<int> > s(boost::in_place(10, 11));
  std::cout << s->back() << std::endl;
}

