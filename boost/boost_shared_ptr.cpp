#include <string>
#include <boost/make_shared.hpp>
#include <vector>
#include <iostream>
#include <exception>

struct classA 
{
  int i; classA(): i(10) 
  { std::cout << "classA ctor\n"; }; 
  classA(const classA&) {std::cout << "classA copy ctor\n";  };
  ~classA() { std::cout << "classA dtor\n"; };
};

struct classB : public classA
{};

using namespace std;

static bool Equal(const boost::shared_ptr<int>& i, const boost::shared_ptr<int>& j)
{
  std::cout << *i << ' ' << *j << ' ' << (*i == *j) << std::endl;

  return *i == *j;
}

static void test1(const boost::shared_ptr<classA>& ) { std::cout << "test1\n"; };

class boost_shared_ptr_A {
public:
  //virtual void sing()=0;
//protected:
  ~boost_shared_ptr_A() { std::cout << "boost_shared_ptr_A dtor\n"; };
};

class boost_shared_ptr_B : public boost_shared_ptr_A {
public:
 // virtual void sing() { std::cout << "Do re mi fa so la"; };
  ~boost_shared_ptr_B() { std::cout << "boost_shared_ptr_B dtor\n"; };
};

static boost::shared_ptr<boost_shared_ptr_A> createA() {
  boost::shared_ptr<boost_shared_ptr_A> p(new boost_shared_ptr_B());
  return p;
}

static void test2() {
  createA();
}

static void test3()
{
  boost::shared_ptr<string> str(new string("shared"));
  boost::shared_ptr<string> str1 = boost::make_shared<string>("shared");
  std::cout << *str << "\n";
  boost::shared_ptr<classB> ptr(new classB());
  const boost::shared_ptr<classB>& cptr = ptr;
  test1(boost::dynamic_pointer_cast<classA>(cptr));
}

static void test4()
{
  FILE* f=fopen("test.txt","r");
  if (f==0) {
    std::cout << "Unable to open file\n";
    return;
  }
  
  boost::shared_ptr<FILE> my_shared_file(f,&fclose);
}

class throwInDtor
{
public:
  void f() { std::cout << "f() in throwInDtor.\n"; };
};

void delThrowInDtor(throwInDtor* p) { throw std::runtime_error("from throwInDtor"); }

struct SParent
{
  int i;
};

static void test5()
{
    boost::shared_ptr<SParent> p1(new SParent());
  boost::shared_ptr<int> p2(p1, &p1->i);
  *p2 = 10;
  std::cout << p1.use_count() << "\n" << p1->i;
}

static void test6()
{
  boost::shared_ptr<int> p;
  if(!p) std::cout << "empty pointer\n";
  p.reset(new int(3));
  if(p) std::cout << "stuffed pointer\n";
}

static void f1(void* p) {}
static void f2(int* p) {}
static void test7()
{
	boost::shared_ptr<void> p1;
	//f1(p1);

	boost::shared_ptr<int> p2;
	//f2(p2);
}

//shared_ptr
void testSharedPtr()
{
  test6();
}