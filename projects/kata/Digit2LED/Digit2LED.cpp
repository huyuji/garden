// digit2led.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cctype>
#include <iostream>
#include <fstream>
using namespace std;

const int length = 9;
const int width = 3;
const int height = 3;
const int size = width*length;

const char led[height][width*10+1] = {
	" _     _  _     _  _  _  _  _ ",
	"| |  | _| _||_||_ |_   ||_||_|",
	"|_|  ||_  _|  | _||_|  ||_| _|",
};

bool checkInput(char* digit)
{
	int i = 0;
	while(digit[i])
	{
		if(!isdigit(digit[i]))
		{
			return false;
		}
		i++;
	}

	return i == length;
}

void digit2led(char* digit, char** output)
{
	if(!checkInput(digit))
	{
		cout << "Illegal input: " << digit << endl;
		return;
	}

	for(int i = 0; i < length; i++)
	{
		int num = digit[i] - '0';
		for(int j = 0; j < height; j++)
		{
			for(int k = 0; k < width; k++)
			{
				output[j][i*width + k] = led[j][num*width + k];
			}
		}
	}
}

void digit2led2file(char* digit, ofstream& file)
{
	char** line = new char*[height];
	for(int i = 0; i < height; i++)
	{
		line[i] = new char[size*2];
	}

	digit2led(digit, line);

	file << digit << endl;
	for(int i = 0; i < height; i++)
	{
		for (int j = 0; j < size; j++)
		{
			file << line[i][j];
		}
		file << endl;
	}

	for(int i = 0; i < height; i++)
	{
		delete line[i];
	}
  delete line;
}

bool generate(char* input)
{
  int checksum = 0;
  for (int i = 0; i < 8; i++)
  {
    int r = rand() % 10;
    input[i] = '0' + r;
    checksum += r * (9 - i);
  }

  if(checksum % 11 > 1)
  {
    input[8] = 11 - checksum % 11 + '0';
    return true;
  }

  return false;
}

int _tmain(int argc, _TCHAR* argv[])
{
	ofstream file("..\\digit2led.txt");
	if(file.fail())
	{
		cout << "fail to create file\n";
		return 0;
	}

  //OCR cases
	digit2led2file("000000000", file);
  digit2led2file("111111111", file);
  digit2led2file("222222222", file);
  digit2led2file("333333333", file);
  digit2led2file("444444444", file);
  digit2led2file("555555555", file);
  digit2led2file("666666666", file);
  digit2led2file("777777777", file);
  digit2led2file("888888888", file);
  digit2led2file("999999999", file);
  digit2led2file("123456789", file);
  digit2led2file("987654321", file);

  //checksum pass cases
  digit2led2file("345882865", file);
  
  char input[10];
  input[9] = 0;
  for(int i = 0; i < 10; i++)
  {
    if(generate(input))
    {
      digit2led2file(input, file);
    }
  }

	return 0;
}