// windows.cpp : Defines the entry point for the console application.
//

#include "tchar.h"
#include <fstream>
#include <iostream>

namespace
{
    void f1()
    {
        const char* fileName = "f";
        const char* content = "end with lf\n";

        std::fstream outfs(fileName, std::fstream::out);
        outfs << content;
        outfs.close();

        const unsigned int size = 64;

        std::fstream infsb(fileName, std::fstream::in | std::fstream::binary);
        char outb[size];
        memset(outb, 0, size);
        infsb.read(outb, size);

        std::fstream infs(fileName, std::fstream::in);
        char out[size];
        memset(out, 0, size);
        infs.read(out, size);

        std::cout << (strlen(out) != strlen(outb)) << std::endl;
    }

    void f2()
    {
        const char* fileName = "f";
        const char* content = "end with lf\n";

        std::fstream outfs(fileName, std::fstream::out | std::fstream::binary);
        outfs << content;
        outfs.close();

        const unsigned int size = 64;

        std::fstream infsb(fileName, std::fstream::in | std::fstream::binary);
        char outb[size];
        memset(outb, 0, size);
        infsb.read(outb, size);

        std::fstream infs(fileName, std::fstream::in);
        char out[size];
        memset(out, 0, size);
        infs.read(out, size);

        std::cout << (strlen(out) == strlen(outb)) << std::endl;
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    f1();
    f2();
    return 0;
}

