#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <iostream>
#include <sstream>
#include <Windows.h>

using namespace boost::gregorian;
using namespace boost::posix_time; 

static time_t to_time_t(const std::string& s)
{
  static time_input_facet in("%Y-%m-%d %H:%M:%S");
  static ptime epoch(boost::gregorian::date(1970, 1, 1));
  static std::stringstream ss;
  ss.imbue(std::locale(std::cout.getloc(), &in));

  ss.str("");
  ss << s;
  ptime t(not_a_date_time);
  ss >> t;
  return (t - epoch).total_seconds();
}

static void test0()
{
  const char* times[] = {
    "2014-01-03 06:40:25",
    "2014-01-03 06:41:10",
  };

 std::cout << sizeof(times) / sizeof(const char*) << std::endl;
  for (unsigned int i = 0; i < sizeof(times) / sizeof(const char*); i++)
  {
    //const std::string& time = times[i];
    //size_t pos = time.find('.');
    //unsigned long long t = to_time_t(time.substr(0, pos));
    //t *= 1000;
    //t += boost::lexical_cast<unsigned int>(time.substr(pos + 1)) / 1000;
    //std::cout << t << std::endl;
    std::cout << to_time_t(times[i]) << std::endl;
  }
}

static void test1()
{
  try
  {
    date d(2013,22,28); //an arbitrary date 
    std::cout << d;
  }
  catch(std::out_of_range&)
  {
    std::cout << "invalid date";
  }
}

static void test3()
{
  date d(not_a_date_time);
  date_input_facet facet("%Y-%m-%d");
  std::cin.imbue(std::locale(std::cout.getloc(), &facet));
  std::cin >> d;
  std::cout << std::endl << d;
}

static void test4()
{
  ptime t(not_a_date_time);
  time_input_facet in("%Y-%m-%d %H:%M:%S");
  time_facet out("%Y-%m-%d %H:%M:%S");
  std::stringstream ss("2013-01-01 18:00:00");
  ss.imbue(std::locale(std::cout.getloc(), &in));
  ss >> t;
  std::cout.imbue(std::locale(std::cout.getloc(), &out));
  std::cout << t;  
}

static void test5()
{
  try
  {
    ptime t(date(1900, 1, 1), time_duration(25,68,1,0));
  }
  catch(std::exception& e)
  {
    std::cout << e.what();
  }
}

class Timer
{
public:
  Timer() { _ms = clock(); };

  unsigned int getElapsedTime()
  {
    clock_t now = clock();
    unsigned int elapses = (float(now - _ms)/CLOCKS_PER_SEC) * 1000;
    _ms = now;

    return elapses;
  };

private:
  clock_t _ms;
};

static boost::uint64_t now()
{
  const boost::posix_time::ptime now( boost::posix_time::microsec_clock::universal_time() );

  const boost::posix_time::ptime time_epoch( boost::gregorian::date(1970, 1, 1) );

  const boost::posix_time::time_duration diff(now - time_epoch);

  return diff.total_milliseconds();
}

static void test6()
{
  Timer t;
  Sleep(1);
  std::cout << t.getElapsedTime() << std::endl;

  boost::uint64_t n = now();
  Sleep(1);
  std::cout << now() - n << std::endl;
}

static void test7()
{
    std::cout << to_iso_string(microsec_clock::universal_time());
}

void testDateTime()
{
  test7();
}
