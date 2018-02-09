#include "stdafx.h"
#include "Pricer.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;

int Pricer::getPrice(string& bookOrder)
{
	stringstream ss(bookOrder);
  vector<int> bookOrderCounts;

	int i;
	while (ss >> i)
	{
		bookOrderCounts.push_back(i);

		while (ss.peek() == ','
          || ss.peek() == ' '
          || ss.peek() == '\t')
    {
		  ss.ignore();
    }
	}

  return getPrice(bookOrderCounts);
}

int Pricer::getPrice(vector<int> bookOrderCounts)
{
  sort(bookOrderCounts.begin(), bookOrderCounts.end());

	vector<int> bookSetCounts;
  int offset = 0;
  for (int i = 0; i < bookOrderCounts.size(); i++)
  {
    bookSetCounts.push_back(bookOrderCounts[i] - offset);
    offset = bookOrderCounts[i];
  }

  reverse(bookSetCounts.begin(), bookSetCounts.end());
  optimize(bookSetCounts);
  return calculatePrice(bookSetCounts);
}

void Pricer::optimize(vector<int>& bookSetCounts)
{
  for(int i = m_bookSetDiscount.size() - 1; i >= 2; i--)
	{
    if(bookSetCounts[i] == 0)
    {
      continue;
    }

		for(int j = i - 2; j >= 0; j--)
		{
      if( (i+1) * m_bookSetDiscount[i] + (j+1) * m_bookSetDiscount[j] 
            >= i * m_bookSetDiscount[i-1] + (j+2) * m_bookSetDiscount[j+1])
      {
        int transferCount = bookSetCounts[i] < bookSetCounts[j] ? bookSetCounts[i] : bookSetCounts[j];
        bookSetCounts[i] -= transferCount;
        bookSetCounts[i-1] += transferCount;
        bookSetCounts[j+1] += transferCount;
        bookSetCounts[j] -= transferCount;
        if(bookSetCounts[i] == 0)
        {
          break;
        }
      }
		}
	}
}

int Pricer::calculatePrice(const vector<int>& bookSetCounts)
{
  int price = 0;
  for (int i = 0; i < bookSetCounts.size(); i++)
  {
    price += (i+1) * bookSetCounts[i] * m_bookSetDiscount[i] * m_bookPrice;
  }

  return price;
}