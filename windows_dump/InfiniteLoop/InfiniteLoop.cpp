// InfiniteLoop.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

void infinite_loop()
{
  while(true)
  {
    int j = 0;
    for(int i = 0; i < 1024 * 1024 * 1024; i++)
    {
      j++;
    }
    printf("looping");
  }
}

int _tmain(int argc, _TCHAR* argv[])
{
  DWORD processAffi = 0;
  DWORD systemAffi = 0;
  GetProcessAffinityMask(NULL, &processAffi, &systemAffi);

  printf("%x, %x\n", processAffi, systemAffi);
  infinite_loop();
	return 0;
}

