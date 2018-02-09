// Kata2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class BankAccount 
{
public:
  BankAccount(char** input):
      faultIndex(-1),
      faultCount(0)
  {
    memset(accountNumber, 0, sizeof(accountNumber));

		for(int i = 0; i < count; i++ )
		{
			accountNumber[i] = transChar(input, i*width);
      if(illegalSymbol == accountNumber[i])
      {
        faultCount++;
        faultIndex = i;
      }
		}    

    if(!isValid())
    {
      guess(input);
    }
  }

  void toString(char* output)
  {
    strcpy(output, accountNumber);
    if(faultCount != 0)
    {
      strcat(output, " ILL");
    }
    else if(!isValid())
    {
      strcat(output, " ERR");
    }
  }

  bool isValid()
  {
    if(faultCount != 0)
    {
      return false;
    }

    return check(accountNumber);
  }
  
private:
  static const int height = 3;
  static const int width = 3;
  static const int count = 9;
  static const int size = count * width;

  static const char illegalSymbol = '?';

  string accountNumber;
  int faultIndex;
  int faultCount;

  bool check(char* num)
  {
    int checksum = 0;
    for(int i = 0; i < 9; i++)
    {
      checksum += (num[i] - '0') * (9 - i);
    }

    return checksum % 11 ? false : true ;
  }

  void guessOne(char** input, int faultIndex)
  {
    char possibility[10];
    memset(possibility, 0, 10);

    guessChar(input, width*faultIndex, possibility);
      
    char trial[count+1];
    memcpy(trial, accountNumber, count);
    trial[count] = 0;

    for(int i = 0; possibility[i] != 0; i++)
    {
      trial[faultIndex] = possibility[i];
      if(check(trial))
      {
        cout << "guess:\t" << trial << endl;
      }
    }
  }

  void guess(char** input)
  {
    if(isValid())
    {
      cout << "account number is valid" << endl;
      return;
    }

    if(faultCount != 1)
    {
      cout << "more than one fault, unable to guess" << endl;
      return;
    }

    if(faultCount == 1)
    {
      guessOne(input, faultIndex);
    }
    else
    {
      for(int i = 0; i < count; i++)
      {
        guessOne(input, count);
      }
    }
  }

  void guessChar(char** line, int offset, char* possibility)
  {
    int index = 0;

	  for(int i = 0; i < height; i++)
	  {
		  for(int j = 0; j < width; j++)
		  {
			  if(line[i][j+offset] == ' ')
			  {
				  line[i][j+offset] = '*';
          char c = transChar(line, offset);
          if(c != '?')
          {
            possibility[index++] = c;
          }
			  }
		  }
	  }
  }

  static char transChar(const char** line, const int offset)
  {
	  int record = 0;
	  for(int i = 0; i < height; i++)
	  {
		  for(int j = 0; j < width; j++)
		  {
			  if(line[i][j+offset] != ' ')
			  {
				  record |= (1 << (i*height+j));
			  }
		  }
	  }

	  switch (record)
	  {
	  case 0x1ea:
		  return '0';
	  case 0x120://100100000:
		  return '1';
	  case 0xF2://011110010:
		  return '2';
	  case 0x1b2://110110010:
		  return '3';
	  case 0x138://100111000:
		  return '4';
	  case 0x19a://110011010:
		  return '5';
	  case 0x1da://111011010:
		  return '6';
	  case 0x122://100100010:
		  return '7';
	  case 0x1fa://111111010:
		  return '8';
	  case 0x1ba://110111010:
		  return '9';
    default:
      return illegalSymbol;
	  }

	  return 0;
  }
};

void test(const char* filePath)
{
  const int height = 3;
  const int width = 3;
  const int size = 27;
  const int count = size/width;

	fstream file(filePath);

	char** line = new char*[height];
	for(int i = 0; i < height; i++)
	{
		line[i] = new char[size*2];
	}
  char* answer = new char[size];

	while(!file.eof())
	{
    memset(answer, 0, size);
    file.getline(answer, size, '\n');
    if(!isdigit(answer[0]))
    {
      cout << "not more test cases." << endl;
      break;
    }

    cout << "expect:\t" << answer << endl;

		for(int i = 0; i < height; i++)
		{
			file.getline(line[i], size*2, '\n');
		}

		BankAccount bankAccount(line);
    char output[count + 5];
    memset(output, 0, sizeof(output));
    bankAccount.toString(output);

    cout << "result:\t";
    cout << output;
		cout << endl;

    if(memcmp(output, answer, count) == 0)
    {
      if(bankAccount.isValid())
      {
        cout << "valid bank account" << endl;
      }
    }
    else
    {
      cout << "failed" << endl;
    }

    cout << endl;
	}

	for(int i = 0; i < height; i++)
	{
		delete line[i];
	}
  delete line;
}

int _tmain(int argc, _TCHAR* argv[])
{
  const char* filePaths[] = {
    "d:\\code\\working\\projects\\kata\\testcase1.txt",
    "d:\\code\\working\\projects\\kata\\testcase2.txt",
    "d:\\code\\working\\projects\\kata\\testcase3.txt",
    "d:\\code\\working\\projects\\kata\\testcase4.txt",
  };

  for(int i = 0; i < sizeof(filePaths); i++)
  {
    cout << "===test file: " << filePaths[i] << "===" << endl;
    test(filePaths[i]);
  }

	return 0;
}

