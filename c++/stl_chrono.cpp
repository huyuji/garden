#include <chrono>
#include <iostream>
#include <assert.h>

namespace
{
    using namespace std::chrono;
    
    void f1()
    {
        milliseconds ms = duration_cast<milliseconds>(
            high_resolution_clock::now().time_since_epoch());
        std::cout << ms.count() << std::endl;
    }
}

void testChrono()
{
    f1();
}