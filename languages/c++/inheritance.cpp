#include <iostream>

namespace
{
  class Level1
  {
  public:
    virtual void f1() { std::cout << "f1 in Level1\n"; };
  };

  class Level2 : public Level1
  {
  public:
    void g() { f1(); };

  private:
    virtual void f1() { std::cout << "f1 in Level2\n"; };
  };

  class Level3 : public Level2
  {
  public:
    void g() 
    { 
      //f1(); // f1 is private, cannot be accessed
    };
  };

  class Level3_2 : public Level2
  {
  public:
    virtual void f1();
    void g()
    {
      f1(); // f1 is undefined
    };
  };

  void f1()
  {
    Level2 d;
    //d.f1(); // f1 is private, cannot be accessed
    d.g();

    Level3 d2;
    d2.g();
  }
}

void testInheritance()
{
  f1();
}