#ifndef ROMANNUMERICAL_H
#define ROMANNUMERICAL_H

#include <string>
using namespace std;

class RomanNumerical
{
public:
	explicit RomanNumerical(int digit): _i(digit){};
	string toString();
	
private:
	bool isOutOfRange();
	string toString(int, int);
	void getSymbols(int index, string& high, string& mid, string& low);
	string _s;
	int _i;
};

#endif //ROMANNUMERICAL_H