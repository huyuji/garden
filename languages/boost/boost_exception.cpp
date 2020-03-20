#include <boost/exception/all.hpp>
#include <boost/exception/errinfo_errno.hpp>
#include <iostream>
#include <string>

struct my_exception: virtual boost::exception {};
typedef boost::error_info<struct tag_err_no, int> err_no;
typedef boost::error_info<struct tag_err_str, std::string> err_msg;

struct my_error: virtual std::exception {};

void testBoostException()
{
  try
  {
    try
    {
      throw my_exception() << err_no(1);
      //errorinfo_api_function,errorinfo_at_line...http://www.boost.org/doc/libs/1_53_0/libs/exception/doc/error_info.html
    }
    catch(boost::exception& e)
    {
      std::cout << *boost::get_error_info<err_no>(e) << std::endl;
      e << err_msg("error message");
      throw;
    }
  }
  catch(boost::exception& e)
  {
    std::cout << *boost::get_error_info<err_msg>(e) << std::endl;
  }

  try
  {
    throw boost::enable_error_info(my_error()) << boost::errinfo_errno(1);
    boost::throw_exception(my_error());
    BOOST_THROW_EXCEPTION(my_error());
  }
  catch(boost::exception& e)
  {
    std::cout << *boost::get_error_info<boost::errinfo_errno>(e) << std::endl;
    std::cout << boost::diagnostic_information(e) << std::endl;
    std::cout << boost::current_exception_diagnostic_information() << std::endl;
    
    std::cout << boost::current_exception_cast<std::exception>()->what() << std::endl;
  }
}
