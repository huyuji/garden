// MemoryLeak.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
  int i = 0;
  int size = 1024 * 1024;

  while(true)
  {
    i++;

    char * c = new char[size];
    Sleep(1000);
    memset(c, 35, size);

    printf("%d allocations now\n", i);
  }

	return 0;
}

