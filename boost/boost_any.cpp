#include <boost/any.hpp>
#include <boost/exception/diagnostic_information.hpp> 
#include <iostream>
#include <vector>
#include <string>
#include <ostream>

struct boost_any_a 
{
  int i; boost_any_a(): i(10) 
  { std::cout << "boost_any_a ctor\n"; }; 
  boost_any_a(const boost_any_a&) {std::cout << "boost_any_a copy ctor\n";  };
  ~boost_any_a() { std::cout << "boost_any_a dtor\n"; };
};

static void test1()
{
  boost::any a(10);
  std::cout << boost::any_cast<int>(a) << std::endl;

  boost::any_cast<int&>(a) = 20;
  std::cout << boost::any_cast<int>(a) << std::endl;

  a = std::string("string");
  {
    boost::any c = boost_any_a();
  }

  try
  {
    std::cout << boost::any_cast<std::string>(a) << std::endl;  
  }
  catch(boost::bad_any_cast&)
  {
    std::cout << boost::current_exception_diagnostic_information() << std::endl;
  }

  {
    boost_any_a* temp = new boost_any_a();
    boost::any b(temp);
    //delete boost::any_cast<boost_any_a*>(a);
  }
}

//static class any_out {
//  boost::any o_;
//
//  struct streamer {
//    virtual void print(std::ostream& o,boost::any& a)=0;
//    virtual streamer* clone()=0;
//    virtual ~streamer() {}
//  };
//
//  template <typename T> struct streamer_imp : public streamer {
//    virtual void print(std::ostream& o,boost::any& a) {
//      o << *boost::any_cast<T>(&a);
//    }
//    virtual streamer* clone() {
//      return new streamer_imp<T>();
//    }
//};
//
//class any_out {
//  streamer* streamer_;
//  boost::any o_;
//public:
//  any_out() : streamer_(0) {}
//  template <typename T> any_out(const T& value) :
//    streamer_(new streamer_imp<T>),o_(value) {}
//  any_out(const any_out& a)
//    : streamer_(a.streamer_?a.streamer_->clone():0),o_(a.o_) {}[1]
//
//  template<typename T> any_out& operator=(const T& r) {
//    any_out(r).swap(*this);
//    return *this;
//  }
//
//any_out& operator=(const any_out& r) {
//  any_out(r).swap(*this);
//  return *this;
//}
//
//~any_out() {
//  delete streamer_;
//}
//any_out& swap(any_out& r) {
//  std::swap(streamer_, r.streamer_);
//  std::swap(o_,r.o_);
//  return *this;
//}
//  friend std::ostream& operator<<(std::ostream& o,any_out& a) {
//    if (a.streamer_) {
//      a.streamer_->print(o,a.o_);
//    }
//    return o;
//  }
//};
//
//static void test2() {
//  std::vector<any_out> vec;
//
//  any_out a(std::string("I do have operator<<"));
//
//  vec.push_back(a);
//  vec.push_back(112);
//  vec.push_back(65.535);
//
//  // Print everything in vector vec
//  std::cout << vec[0] << "\n";
//  std::cout << vec[1] << "\n";
//  std::cout << vec[2] << "\n";
//
//  a=std::string("This is great!");
//  std::cout << a;
//}

void testAny()
{
}
