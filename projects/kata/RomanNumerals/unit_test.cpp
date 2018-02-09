// unit_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gtest/gtest.h"
#include "RomanNumerical.h"

int main(int argc, _TCHAR** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

class TestRomanNumerical : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
	}
};

TEST_F(TestRomanNumerical, One)
{
	EXPECT_STREQ("I", (new RomanNumerical(1))->toString().c_str());
}

TEST_F(TestRomanNumerical, Two)
{
	EXPECT_STREQ("II", (new RomanNumerical(2))->toString().c_str());
}

TEST_F(TestRomanNumerical, Four)
{
	EXPECT_STREQ("IV", (new RomanNumerical(4))->toString().c_str());
}

TEST_F(TestRomanNumerical, Five)
{
	EXPECT_STREQ("V", (new RomanNumerical(5))->toString().c_str());
}

TEST_F(TestRomanNumerical, Six)
{
	EXPECT_STREQ("VI", (new RomanNumerical(6))->toString().c_str());
}

TEST_F(TestRomanNumerical, Nine)
{
	EXPECT_STREQ("IX", (new RomanNumerical(9))->toString().c_str());
}

TEST_F(TestRomanNumerical, Ten)
{
	EXPECT_STREQ("X", (new RomanNumerical(10))->toString().c_str());
}

TEST_F(TestRomanNumerical, Eleven)
{
	EXPECT_STREQ("XI", (new RomanNumerical(11))->toString().c_str());
}

TEST_F(TestRomanNumerical, Fourteen)
{
	EXPECT_STREQ("XIV", (new RomanNumerical(14))->toString().c_str());
}

TEST_F(TestRomanNumerical, D369)
{
	EXPECT_STREQ("CCCLXIX", (new RomanNumerical(369))->toString().c_str());
}