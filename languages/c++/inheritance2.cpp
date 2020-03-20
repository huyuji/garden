#include<iostream>

namespace
{
class L1
{
public:
    L1(int i) { std::cout << "L1 ctor called with " << i << ".\n"; }
};

class L2: public L1
{
    using L1::L1;
};

void f1() {
    L2 o(32);
}
}

void testInheritance2() {
    f1();
}