#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

namespace
{
  // You can convert normal iterators into reverse iterators. 
  // Naturally, the iterators must be bidirectional iterators, 
  // but note that the logical position of an iterator is moved during the conversion.
  // This behavior is a consequence of the fact that ranges are half open.
  // To specify all elements of a container, you must use the position after the last argument.
  // However, for a reverse iterator, this is the position before the first element.
  // Unfortunately, such a position may not exist.
  // Containers are not required to guarantee that the position before their first element is valid.
  // Consider that ordinary strings and arrays might also be containers, 
  //   and the language does not guarantee that arrays don¡¯t start at address zero.
  // As a result, the designers of reverse iterators use a trick : 
  // They ¡°physically¡± reverse the ¡°half - open principle.¡± 
  // Physically, in a range defined by reverse iterators, the beginning is not included, whereas the end is.
  // However, logically, they behave as usual.
  // Thus, there is a distinction between the physical position that defines the element 
  //   to which the iterator refers and the logical position that 
  //   defines the value to which the iterator refers.
  // The question is, what happens on a conversion from an iterator to a reverse iterator ? 
  // Does the iterator keep its logical position(the value) or its physical position(the element) ? 
  // As the example below shows, the latter is the case.
  // Thus, the value is moved to the previous element.
  void f1()
  {
    std::vector<int> v = { 1, 2, 3, 4, 5 };
    auto it = v.begin() + 2;
    std::cout << *it << std::endl; // 3
    std::vector<int>::reverse_iterator rit(it);
    std::cout << *rit << std::endl; // 2
  }

  // use the base() member of reverse_iterator to get back the normal iterator
  void f2()
  {
    std::vector<int> v = { 1, 2, 3 };
    auto rit = v.rbegin();
    auto it = rit.base();
    std::cout << (it == v.end()) << std::endl;
  }
}

void testStlReverseIterator()
{
  f1();
}