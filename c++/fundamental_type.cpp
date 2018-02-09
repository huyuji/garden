#include <iostream>
#include <string>

namespace
{
  /*
  1.#INF
  1.79769e+308
  2.22507e-308
  */
  void f1()
  {
    std::cout << std::numeric_limits<double>::infinity() << std::endl;
    std::cout << std::numeric_limits<double>::max() << std::endl;
    std::cout << std::numeric_limits<double>::min() << std::endl;
  }

  int u1(char x) {
    char y = x;
    return ++x > y;
  }

  void f2()
  {
    int i;
    printf("%d %d\n", CHAR_MIN, CHAR_MAX);
    for (i = CHAR_MIN; i <= CHAR_MAX; i++) {
      printf("%d ", u1(i));
      if ((i & 31) == 31) printf("\n");
    }
  }

  std::string u2(std::string& s = std::string("s"))
  {
    return s;
  }

  void f3()
  {
    std::cout << u2() << std::endl;
  }

  void u3(unsigned int) { std::cout << "function takes unsigned int" << std::endl; }
  void u3(unsigned int *) { std::cout << "function takes unsigned int *" << std::endl; }
  void u3(void *) { std::cout << "function takes void *" << std::endl; }
  void u3(std::nullptr_t) { std::cout << "function takes nullptr" << std::endl; }

  // NULL converts to integral types while nullptr converts to any pointer type
  void f4()
  {
    unsigned int ui = 13;
    unsigned int * p = NULL;
    int * np = nullptr;

    u3(ui);
    u3(p);
    u3(np);
  }

  void f5()
  {
    std::cout << (unsigned int)nullptr << std::endl;
  }

  void f6()
  {
    std::cout << sizeof(wchar_t) << std::endl;
  }
}

void testFundamentalType()
{
  f6();
}