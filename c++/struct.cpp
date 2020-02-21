#include<iostream>

namespace
{
class C
{
public:
    int i;
};

struct I
{
    int i;
};

void f1() {
    C c1;
    c1.i = 1;
    C c2;
    c2.i = 1;

    I i1;
    i1.i = 1;
    I i2;
    i2.i = 1;
}
}

void testStruct(){
    f1();
}
