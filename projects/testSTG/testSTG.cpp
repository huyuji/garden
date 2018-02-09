// testSTG.cpp : Defines the entry point for the console application.
//

#include "windows.h"
#include <iostream>
#include "Objbase.h"
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT hr = 0;
	enum Operations
	{
		UNDEFINED,
		CREATE,
		OPEN,
		RECREATE,
		READ,
		WRITE,
	};

	int operation = UNDEFINED;
	int action = UNDEFINED;

	bool parameterFault = false;
	WCHAR* faultMessage = NULL;
	WCHAR* srcfile = NULL;
	WCHAR* logfilename = L"log";
	WCHAR* streamname = L"stream";

	//get parameters
	for(int i = 1; i < argc; i++)
	{
		if(wcscmp(argv[i], L"-create") == 0)
		{
			if(operation != UNDEFINED)
			{
				parameterFault = true;
				faultMessage = L"dual operation";
				break;
			}
			operation = CREATE;
		}
		else if(wcscmp(argv[i], L"-open") == 0)
		{
			if(operation != UNDEFINED)
			{
				parameterFault = true;
				faultMessage = L"dual operation";
				break;
			}
			operation = OPEN;
		}
		else if(wcscmp(argv[i], L"-read") == 0)
		{
			if(action != UNDEFINED)
			{
				parameterFault = true;
				faultMessage = L"dual operation";
				break;
			}
			action = READ;
		}
		else if(wcscmp(argv[i], L"-write") == 0)
		{
			if(action != UNDEFINED)
			{
				parameterFault = true;
				faultMessage = L"dual operation";
				break;
			}
			action = WRITE;
		}
		else if(wcscmp(argv[i], L"-file") == 0)
		{
			i++;
			if(i >= argc)
			{
				parameterFault = true;
				faultMessage = L"missing source file";
				break;
			}
			srcfile = argv[i];
		}
	}

	//check parameters
	if(parameterFault)
	{
		std::cout << faultMessage << std::endl;
		return -1;
	}

	IStorage* pIStorage = NULL;
	IStorage* pIStorage1 = NULL;
	IStream* pIStream = NULL;
	BYTE* buffer = NULL;
	LARGE_INTEGER move;
	HANDLE srcFileHandle = INVALID_HANDLE_VALUE;

	__try
	{
		if (operation == CREATE)
		{
			std::cout << "create\n";

			hr = StgCreateDocfile(logfilename,
									STGM_CREATE  | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_DIRECT,
									0,
									&pIStorage);
			if(FAILED(hr))
			{
				std::cout << "failed to create storage object\n";
				return -1;
			}

			hr = pIStorage->CreateStorage(L"storage1",
										  STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_DIRECT,
										  0,
										  0,
										  &pIStorage1);
			if(FAILED(hr))
			{
				std::cout << "failed to create storage1\n";
				return -1;
			}

			hr = pIStorage1->CreateStream(streamname,
										  STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_DIRECT,
										  0,
										  0,
										  &pIStream);
			if(FAILED(hr))
			{
				std::cout << "failed to init stream\n";
				return -1;		
			}

			const int buffersize = 1024*1024;
			buffer = new BYTE[buffersize];
			memset(buffer, 0, buffersize);
			DWORD bytes = 0;

			if(srcFileHandle == INVALID_HANDLE_VALUE && srcfile != NULL)//write file data into stream
			{
				srcFileHandle = CreateFile(	srcfile, 
											GENERIC_READ, 
											FILE_SHARE_READ, 
											NULL, 
											OPEN_EXISTING, 
											FILE_ATTRIBUTE_NORMAL, 
											NULL);
				if(INVALID_HANDLE_VALUE == srcFileHandle)
				{
					std::cout << "failed to open source file\n";
					return -1;
				}

				while(ReadFile(srcFileHandle, buffer, buffersize, &bytes, NULL) && bytes)
				{
					DWORD bytesWritten = 0;
					hr = pIStream->Write(buffer, bytes, &bytesWritten);
					if(FAILED(hr) || (bytes != bytesWritten))
					{
						std::cout<< "write stream fault\n";
						return -1;
					}
					bytes = 0;
				}
			}
			else//write all 0/1 data into stream
			{
				char str[] = "this is a test string";
				memcpy(buffer, (void*)str, sizeof(str));
				for(int i = 0; i < 100; i++)
				{
					hr = pIStream->Write(buffer, buffersize, &bytes);
					if(FAILED(hr) || (bytes != buffersize))
					{
						std::cout<< "write fault\n";
					}
				}
			}

			std::cout << "pause...press enter";
			getchar();
		}
		else if(operation == OPEN)
		{
			std::cout << "open\n";

			hr = StgOpenStorage(logfilename, 
								NULL,
								STGM_DIRECT  | STGM_READWRITE | STGM_SHARE_EXCLUSIVE ,
								NULL,
								0,
								&pIStorage);

			if(FAILED(hr))
			{
				std::cout << "failed to open storage\n";
				return -1;
			}

			hr = pIStorage->OpenStorage(L"storage1", 
										NULL,
										STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
										NULL,
										0,
										&pIStorage1);

			if(FAILED(hr))
			{
				std::cout << "failed to open storage1\n";
				return -1;
			}

			hr = pIStorage1->OpenStream(streamname,
			  NULL,
			  STGM_READWRITE | STGM_SHARE_EXCLUSIVE ,
			  0,
			  &pIStream);

 			if(FAILED(hr))
			{
				std::cout << "failed to open stream\n";
				return -1;
			}

			memset(&move, 0, sizeof(move));
			hr = pIStream->Seek(move, STREAM_SEEK_END, NULL);
			if(FAILED(hr))
			{
				std::cout << "failed to go to stream end\n";
				return -1;
			}

			const int buffersize = 1024*1024;
			buffer = new BYTE[buffersize];
			memset(buffer, 0, buffersize);
			DWORD byteswritten;

			for(int i = 0; i < 256; i++)
			{
				hr = pIStream->Write(buffer, buffersize, &byteswritten);
				if(FAILED(hr) || (byteswritten != buffersize))
				{
					std::cout<< "write fault\n";
				}
			}

			std::cout << "pause...press enter";
			getchar();
		}
		else if (action == WRITE)
		{
			std::cout << "write\n";

			hr = StgCreateDocfile(logfilename,
									STGM_CREATE  | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_DIRECT,
									0,
									&pIStorage);
			if(FAILED(hr))
			{
				std::cout << "failed to create storage object\n";
				return -1;
			}

			hr = pIStorage->CreateStorage(L"storage2",
										  STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_DIRECT,
										  0,
										  0,
										  &pIStorage1);
			if(FAILED(hr))
			{
				std::cout << "failed to create storage1\n";
				return -1;
			}

			hr = pIStorage1->CreateStream(streamname,
										  STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_DIRECT,
										  0,
										  0,
										  &pIStream);
			if(FAILED(hr))
			{
				std::cout << "failed to init stream\n";
				return -1;		
			}

			const int buffersize = 1024*1024;
			buffer = new BYTE[buffersize];
			memset(buffer, 0, buffersize);
			DWORD bytes = 0;

			char str[] = "this is test string two";
			memcpy(buffer, (void*)str, sizeof(str));
			hr = pIStream->Write(buffer, buffersize, &bytes);
			if(FAILED(hr) || (bytes != buffersize))
			{
				std::cout<< "write fault\n";
			}

			std::cout << "pause...press enter";
			getchar();
		}
	}
	__finally
	{
		if(pIStream != NULL)
		{
			pIStream->Release();
			pIStream = NULL;
		}
		if(pIStorage1 != NULL)
		{
			pIStorage1->Release();
			pIStorage1 = NULL;
		}
		if(pIStorage != NULL)
		{
			pIStorage->Release();
			pIStorage = NULL;
		}
		
		if(buffer != NULL)
		{
			delete buffer;
		}

		if(INVALID_HANDLE_VALUE != srcFileHandle)
		{
			CloseHandle(srcFileHandle);
		}
	}

	return 0;
}

