#include <boost/regex.hpp> 
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>

static void f1(boost::regex& expression, std::string& arguments)
{
  boost::cmatch what; 
  
  if(boost::regex_match(arguments.c_str(), what, expression)) 
  { 
    for(size_t i = 1; i < what.size(); i++)
    {
      unsigned int n = what[i].str().empty() ? 0 : boost::lexical_cast<unsigned int>(what[i].str());
      std::cout << n << '|';
    }
  } 
  else
  {
    std::cout << "no match: ";
    for(size_t i = 1; i < what.size(); i++)
    {
      std::cout << what[i] << '|';
    }
  }

  std::cout << std::endl;
}

static void test1()
{
  boost::regex expression("(?:/metrics/value/hist)(?:(\\s+-id\\s+(?:\"[^\"]+\")|(?:[^\"]+))|(\\s+-historyEnabled)|(\\s+-realtimeEnabled))?(\\s+show)(\\s+-detail)?(\\s+-output\\s+(?:(?:csv)|(?:nvp)|(?:table)))?", boost::regex::perl|boost::regex::icase); 
  
  std::string arguments;
  
  arguments = "/metrics/value/hist  SHOW";
  f1(expression, arguments);

  arguments = "/metrics/value/hist -id \"cifs.read, cifs.write\" show";
  f1(expression, arguments);

  arguments = "/metrics/value/hist -id cifs.read show";
  f1(expression, arguments);

  arguments = "/metrics/value/hist -historyEnabled show";
  f1(expression, arguments);

  arguments = "/metrics/value/hist -realtimeEnabled show";
  f1(expression, arguments);
}

static void test2()
{
  std::string s1("(\\d{4})-(\\d{1,2})-(\\d{1,2})()()()");
  std::string s2("()()()(\\d{1,2}):(\\d{1,2}):(\\d{1,2})");
  std::string s3(s1 + "(?: |T)?" + s2);
  boost::regex expression(s2, boost::regex::perl); 
  
  std::string arguments;
  
  arguments = "1:2:3";
  f1(expression, arguments);
}

static void test3() 
{
  boost::regex reg("(Colo)(u)(r)",
    boost::regex::icase|boost::regex::perl);
  
  std::string s="Colour, colours, color, colourize";

  s=boost::regex_replace(s,reg,"$1$3");
  std::cout << s;
}

class regex_callback {
  int sum_;
public:
  regex_callback() : sum_(0) {}

  template <typename T> void operator()(const T& what) {
    sum_+=atoi(what[1].str().c_str());
  }

  int sum() const {
    return sum_;
  }
};

static void test4() {
  boost::regex reg("(\\d+),?");
  std::string s="1,1,2,3,5,8,13,21";

  boost::sregex_iterator it(s.begin(),s.end(),reg);
  boost::sregex_iterator end;

  regex_callback c;
  int sum=for_each(it,end,c).sum();
  std::cout << sum << std::endl;
}

static void test5() {
  boost::regex reg("/");
  std::string s="Split/Values/Separated/By/Slashes,";
  std::vector<std::string> vec;
  boost::sregex_token_iterator it(s.begin(),s.end(),reg,-1);
  boost::sregex_token_iterator end;
  while (it!=end) 
    vec.push_back(*it++);

  assert(vec.size()==std::count(s.begin(),s.end(),'/')+1);
  assert(vec[0]=="Split");
}

static void test6()
{
  boost::cmatch what; 
  if(boost::regex_match(
       "error(1) on device(spa): An Exception was caught.  unknown stat: 'cifs.smb1.basic'",
        what, 
        boost::regex(".*An Exception was caught.  unknown stat: '(.*)'", boost::regex::perl))) //case sensitive
  { 
    for(size_t i = 0; i < what.size(); i++)
    {
      std::cout << what[i].str() << std::endl;
    }
  } 
  else
  {
    std::cout << "not match\n";
  }
}

static void test7()
{
    boost::cmatch what; 
    if(boost::regex_match("error(1) on device(spa): An Exception was caught.  Name not found '00000107'", what, 
        boost::regex(".*An Exception was caught.  Name not found: '(.*)'", 
          boost::regex::perl))) //case sensitive
    { 
      std::cout << what[1].str() << std::endl;
      return;
    }
}

void testRegex()
{
  test7();
}
