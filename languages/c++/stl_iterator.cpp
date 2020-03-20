#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

namespace
{
  // random access iterator support arithmetic
  void f1()
  {
    std::vector<int> v = { 1, 2, 3, 4, 5 };
    auto it = v.begin();
    std::cout << it[3] << std::endl;
    std::cout << (it + 2)[-1] << std::endl;
  }

  // bidirectional iterator support only ++/--
  void f2()
  {
    std::list<int> l = { 1, 2, 3, 4, 5 };
    auto it = l.begin();
    std::cout << *++it << std::endl;
  }

  // iterators of vectors, arrays, and strings might be implemented as ordinary pointers. 
  // For all fundamental data types, such as pointers, you are not allowed to modify temporary values. 
  // For structures and classes, however, doing so is allowed. 
  // Thus, if the iterator is implemented as an ordinary pointer, the compilation fails; 
  // if implemented as a class, it succeeds.
  void f3()
  {
    std::vector<int> coll;

    // NONPORTABLE version
    if (coll.size() > 1)
    {
      std::sort(++coll.begin(), coll.end());
    }

    // PORTABLE version since C++11
    if (coll.size() > 1) {
      std::sort(std::next(coll.begin()), coll.end());
    }
  }

  // advance(), next() and prev() does not check whether it crosses the boundary of a sequence
  // the following code will compile and run (in Release version) without error
  void f4()
  {
    std::vector<int> v = { 1, 2, 3, 4, 5 };
    auto lt = std::next(v.end());
    lt = std::prev(v.begin());
    lt = v.end();
    std::advance(lt, 2);
    lt = v.begin();
    std::advance(lt, -2);
  }

  class RandomIter : public std::iterator < std::random_access_iterator_tag, int >
  {
  public:
    RandomIter& operator + (unsigned int i)
    {
      std::cout << "+ of RandomIter called\n";
      return *this;
    }
    
    RandomIter& operator += (unsigned int i)
    {
      std::cout << "+= of RandomIter called\n";
      return *this;
    }

    RandomIter& operator ++ ()
    {
      std::cout << "++ of RandomIter called\n";
      return *this;
    }

    RandomIter operator ++(int)
    {
      std::cout << "++(int) of RandomIter called\n";
      return *this;
    }
  };

  // Due to the use of iterator traits, 
  // the function advance()/distance() always uses the best implementation, 
  // depending on the iterator category.
  // For random - access iterators, it simply calls pos += n.
  // For all other iterators, it calls ++pos n times(or --pos if n is negative).
  void f5()
  {
    RandomIter it;
    std::advance(it, 1);
    std::advance(it, 2);
  }

  void f6()
  {
    std::vector<int> v = { 1 };
    std::vector<int> v1 = { 5 };

    auto it = v.begin();
    std::cout << *it << std::endl;

    std::iter_swap(it, v1.begin());
    std::cout << *it << std::endl;

    std::vector<double> v2 = { 2.1 };
    std::list<int> l1 = { 3 };
    std::list<double> l2 = { 4.1 };

    // the following will not compile
    //std::iter_swap(it, v2.begin());
    //std::iter_swap(it, l1.begin());
    //std::iter_swap(it, l2.begin());
  }
}

void testStlIterator()
{
  f6();
}