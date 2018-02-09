#pragma once
#ifndef id1A6C7CB0_32BD_4ACE_BDAD4FDD1B0FA29D
#define id1A6C7CB0_32BD_4ACE_BDAD4FDD1B0FA29D

#include <exception>

namespace AMT
{

class AMTException : public std::exception
{
public:
    explicit AMTException(const std::string& what) : what_(what)
    {
    };

    const char* what() const { return what_.c_str(); };

private:
    const std::string what_;
};

}//end of namespace AMT

#endif // header
