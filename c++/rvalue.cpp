#include<iostream>

namespace
{
void f1() {
    int i = 10;
    // int& ir = (i + 1); // clang 6.0.0 error: non-const lvalue reference to type 'int' cannot bind to a temporary of type 'int'
    int&& ir = (i + 1);
    ir = 12;
    std::cout << ir << '\n';
}
}

void testRvalue() {
    f1();
}
