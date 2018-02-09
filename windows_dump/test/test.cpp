// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>

__int64 print(int a)
{
  std::cout<< "32\n";
  return 32;
}

__int64 print(__int64 a)
{
  std::cout<< "64\n";
  return 64;
}

class A{
public:
  int a;
//  void a(){};
};

void calc(int&, int&, int*)
{
  std::cout<< "calc32\n";
}

void calc(__int64&, __int64&, __int64*)
{
  std::cout << "calc64\n";
}

__int64 DeltaCount(__int64 earliercount, __int64 latercount)
{  
  return( (latercount - earliercount) & (0xFFFFFFFFL)); 
}


void test1()
{
  int a = 32;
  int b = 32;
  int c = 0;

  __int64 d = 16;
  __int64 e = 16;
  __int64 f = 0;

  int g = 0;
  long h = 0;

  calc(a, b, &c);
  calc(d, e, &f);

  short i = 1;
  unsigned int j = 1;
}

void test2()
{
  int z = 30 * 24 * 60 * 60 * 1000 / 20000;
  std::cout<< std::hex << z << std::endl;
}

int divide(unsigned int a)
{
  return a / 512;
}

void test3()
{
  unsigned int a = 0xffffffff;
  std::cout << std::hex << divide(a) << std::endl;
}

void test4()
{
  char* c = NULL;
  try
  {
    c[0] = 'a';
  }
  catch(...)
  {
    std::cout << "exception caught" << std::endl;
  }
}

int _tmain(int argc, _TCHAR* argv[])
{
  //test1();
  //test2();
  //test3();

  test4();
  //std::cout << sizeof(long double) << std::endl;
	return 0;
}

