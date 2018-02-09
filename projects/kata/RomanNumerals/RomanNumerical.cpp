#include "stdafx.h"
#include "RomanNumerical.h"

bool RomanNumerical::isOutOfRange()
{
	return _i >= 3999;
}

string RomanNumerical::toString()
{
	if(isOutOfRange())
	{
		return "Out of range";
	}

	int temp_i = _i;
	string temp_s;

	int index = 1;
	while(temp_i)
	{
		int digit = temp_i % 10;
		temp_s = toString(digit, index) + temp_s;
		index *= 10;
		temp_i /= 10;
	}

	_s = temp_s;

	return _s;
}

void RomanNumerical::getSymbols(int index, string& high, string& mid, string& low)
{
	if(index == 1)
	{
		high = "X";
		mid = "V";
		low = "I";
	}
	else if(index == 10)
	{
		high = "C";
		mid = "L";
		low = "X";
	}
	else if (index == 100)
	{
		high = "M";
		mid = "D";
		low = "C";
	}
}

string RomanNumerical::toString(int digit, int index)
{
	if(digit == 0)
	{
		return "";
	}

	string high;
	string mid;
	string low;

	getSymbols(index, high, mid, low);

	string temp_s;
	if(digit == 9)
	{
		temp_s = low+high;
	}
	else if(digit == 4)
	{
		temp_s = low+mid;
	}
	else
	{
		if(digit >= 5)
		{
			temp_s = mid;
			digit -= 5;
		}

		while(digit--)
		{
			temp_s += low;
		}
	}

	return temp_s;
}