#ifndef _Pricer_h_
#define _Pricer_h_

#include <string>
#include <vector>
using namespace std;

class Pricer
{
public:
  Pricer(const vector<int>& bookSetDiscount, int bookPrice) : 
    m_bookSetDiscount(bookSetDiscount),
    m_bookPrice(bookPrice) {};
  int getPrice(string&);
  void optimize(vector<int>& bookSetCounts);
  int getPrice(vector<int> bookOrderCounts);

private:
  int calculatePrice(const vector<int>& bookSetCounts);

  vector<int> m_bookSetDiscount;
  int m_bookPrice;
};

#endif //_Pricer_h_