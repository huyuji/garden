#include <iostream>

namespace
{

class A
{
public:
    int i1;
    int i2;

    A() {
        i1 = 1;
        i2 = 3;
        pi1 = 2;
        pi2 = 4;
    }
private:
    int pi1;
    int pi2;
};

class B
{
public:
    int i1;
    int i2;
    static int si;

    B() {
        i1 = 1;
        i2 = 3;
    }
};
int B::si = 99;

void f1()
{
    //auto [a, b, c, d] = A{}; // error: cannot decompose non-public member 'pi1' of '(anonymous namespace)::A
    //auto [a, b] = A{}; // error: type '(anonymous namespace)::A' decomposes into 4 elements, but only 2 names were provided
    //auto [a, b, c] = B{}; // error: type '(anonymous namespace)::B' decomposes into 2 elements, but 3 names were provided
    auto [a, b] = B{};
    std::cout << a << '\n' << b << '\n';
}

}

void testStructuredBinding()
{
    f1();
}
