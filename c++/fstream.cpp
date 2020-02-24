#include <iostream>
#include <fstream>

namespace
{
void f1() {
    std::fstream fs1;
    std::fstream fs2 = fs1; // error: call to deleted constructor of 'std::fstream' (aka 'basic_fstream<char>')
}
}

void testFstream() {
    f1();
}
