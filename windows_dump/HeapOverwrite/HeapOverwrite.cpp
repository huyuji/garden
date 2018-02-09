// HeapOverwrite.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
  char * s = new char[4];
  scanf("%s", s);

  printf("%s\n", s);
	return 0;
}

