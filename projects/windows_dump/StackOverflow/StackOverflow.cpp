// StackOverflow.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
void f1();
void f2();

void f1()
{
  int a = 1;
  printf("f1 1 %d\n", a);

  f2();
  printf("f1 2 %d\n", a);
}

void f2()
{
  char s[4];
  sprintf(s, "overflow");
}

int _tmain(int argc, _TCHAR* argv[])
{
  f1();
	return 0;
}

