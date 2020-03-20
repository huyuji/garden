// deadlock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

int count = 0;
HANDLE ghMutex1; 
HANDLE ghMutex2; 

DWORD WINAPI fthread1(
  __in  LPVOID lpParameter
)
{
  DWORD dwWaitResult1 = WaitForSingleObject( 
      ghMutex1,    // handle to mutex
      INFINITE);  // no time-out interval

  if(dwWaitResult1 != WAIT_OBJECT_0)
  {
    printf("wait for 1 error\n");
    return 0;
  }

  count = 1;

  while(count == 1)
    ;

  DWORD dwWaitResult2 = WaitForSingleObject( 
      ghMutex2,    // handle to mutex
      INFINITE);  // no time-out interval

  printf("thread 1 finished\n");
  return 0;
}

DWORD WINAPI fthread2(
  __in  LPVOID lpParameter
)
{
  DWORD dwWaitResult2 = WaitForSingleObject( 
      ghMutex2,    // handle to mutex
      INFINITE);  // no time-out interval

  if(dwWaitResult2 != WAIT_OBJECT_0)
  {
    printf("wait for 2 error\n");
    return 0;
  }

  while(count != 1)
    ;

  count = 2;

  DWORD dwWaitResult1 = WaitForSingleObject( 
      ghMutex1,    // handle to mutex
      INFINITE);  // no time-out interval

  printf("thread 2 finished\n");
  return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
  ghMutex1 = CreateMutex( 
      NULL,              // default security attributes
      FALSE,             // initially not owned
      NULL);             // unnamed mutex

  if (ghMutex1 == NULL) 
  {
      printf("CreateMutex 1 error: %d\n", GetLastError());
      return 0;
  }

  ghMutex2 = CreateMutex( 
      NULL,              // default security attributes
      FALSE,             // initially not owned
      NULL);             // unnamed mutex

  if (ghMutex2 == NULL) 
  {
      printf("CreateMutex 2 error: %d\n", GetLastError());
      return 0;
  }

  DWORD threadId1 = 0;
  HANDLE hthread1 = CreateThread( 
              NULL,       // default security attributes
              0,          // default stack size
              (LPTHREAD_START_ROUTINE) fthread1, 
              NULL,       // no thread function arguments
              0,          // default creation flags
              &threadId1); // receive thread identifier

  if( hthread1 == NULL )
  {
      printf("CreateThread 1 error: %d\n", GetLastError());
      return 0;
  }

  DWORD threadId2 = 0;
  HANDLE hthread2 = CreateThread( 
              NULL,       // default security attributes
              0,          // default stack size
              (LPTHREAD_START_ROUTINE) fthread2, 
              NULL,       // no thread function arguments
              0,          // default creation flags
              &threadId2); // receive thread identifier

  if( hthread2 == NULL )
  {
      printf("CreateThread 2 error: %d\n", GetLastError());
      return 0;
  }

  printf("threads started\n");
  getchar();

	return 0;
}

