// Potter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gtest/gtest.h"
#include "Pricer.h"
#include <vector>

int main(int argc, _TCHAR** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

class TestPotter : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
    discount.push_back(100);
    discount.push_back(95);
    discount.push_back(90);
    discount.push_back(80);
    discount.push_back(75);

    pricer = new Pricer(discount, 8);
	}

public:
  std::vector<int> discount;
  Pricer* pricer;
};

TEST_F(TestPotter, One)
{
	EXPECT_EQ(800, pricer->getPrice(string("1,0,0,0,0")));
  EXPECT_EQ(800, pricer->getPrice(string("0,1,0,0,0")));
  EXPECT_EQ(800, pricer->getPrice(string("0,0,1,0,0")));
  EXPECT_EQ(800, pricer->getPrice(string("0,0,0,1,0")));
  EXPECT_EQ(800, pricer->getPrice(string("0,0,0,0,1")));

  EXPECT_EQ(1600, pricer->getPrice(string("0,0,0,0,2")));
  EXPECT_EQ(1600, pricer->getPrice(string("0,0,2,0,0")));

  EXPECT_EQ(2*8*95, pricer->getPrice(string("0,0,0,1,1")));
  EXPECT_EQ(2*8*95, pricer->getPrice(string("0,1,0,1,0")));

  EXPECT_EQ(3*8*90, pricer->getPrice(string("0,0,1,1,1")));
  EXPECT_EQ(4*8*80, pricer->getPrice(string("1,1,1,1,0")));
  EXPECT_EQ(5*8*75, pricer->getPrice(string("1,1,1,1,1")));
  
  EXPECT_EQ(2 * (8 * 4 * 80), pricer->getPrice(string("2,2,2,1,1")));
  EXPECT_EQ(3 * (8 * 5 * 75) + 2 * (8 * 4 * 80), pricer->getPrice(string("5,5,4,5,4")));
  EXPECT_EQ(3 * (8 * 4 * 80), pricer->getPrice(string("2,3,3,3,1")));

  EXPECT_EQ(2 * (8 * 5 * 75) + 1 * (8 * 2 * 95), pricer->getPrice(string("2,3,3,2,2")));
}