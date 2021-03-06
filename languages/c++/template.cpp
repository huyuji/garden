#include <iostream>
#include <string>

namespace
{
  //TC-complete
  template <int N> struct template_factorial
  {
    enum { val = template_factorial<N - 1>::val * N };
  };

  template<>
  struct template_factorial < 0 >
  {
    enum { val = 1 };
  };

  void f1()
  {
    //Note this value is generated at compile time.
    //Also note that most compilers have a limit on the depth of the recursion available.
    std::cout << template_factorial<4>::val << "\n";
  }

  template <typename T>
  class MyClass {
  private:
    T value;
  public:
    // template copy ctor
    template <typename X>
    MyClass(MyClass<X> x) : value(x.getValue())
    {
      std::cout << "template copy ctor called\n";
    }

    // template ctor
    // this does not suppress the implicit declaration of the copy constructor. 
    // If the type matches exactly, 
    // the implicit copy constructor is generated and called.
    template <typename X>
    MyClass(X x) : value(x)
    {
      std::cout << "template ctor called\n";
    }

    // member template 
    // allows different template types
    template <typename X> 
    void assign(const MyClass<X>& x) {
      // x is of a different type than this now,
      // so only public member can be accessed
      value = x.getValue();
    }
    T getValue() const {
      return value;
    }
  };

  void f2()
  {
    MyClass<double> d(1.2);
    MyClass<double> d1(d); // implicit copy constructor will be called instead of template copy constructor
    MyClass<int> i(1.2);
    std::cout << i.getValue() << std::endl;
    MyClass<double> d2(i);
    d.assign(d);  // OK
    d.assign(i);  // OK (int is assignable to double)
  }

  template<typename T> constexpr T number = T(3.14);
  void variableTemplate()
  {
    std::cout << number<int> << '\n' << number<double> << '\n';
  }

  template<typename ...Args>
  void printer(Args... args)
  {
    (std::cout << ... << args) << '\n'; // parenthesis is required for the fold expression is be recognized
  }

  template<typename ...Args>
  int sum(Args&&... args)
  {
    /**
     * Error: operator with precedence below cast
     * clang 6.0.0 error: expression not permitted as operand of fold expression
     */
    //return (args + ... + 1 * 2);
    return (args + ... + (1 * 2));
  }

  void foldExpression()
  {
    std::cout << sum(1, 2.1) << '\n';
    printer(1, 'a', "string", 3.14);
  }
}

void testTemplate()
{
  foldExpression();
}