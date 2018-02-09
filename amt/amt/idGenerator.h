#pragma once
#ifndef id7C10B977_D330_4DE3_A65714E3323A257E
#define id7C10B977_D330_4DE3_A65714E3323A257E

#include <boost\thread.hpp>

namespace AMT
{

class IdGenerator
{
public:
    IdGenerator() : id_(1)
    {
    };

    unsigned int next()
    {
        boost::unique_lock<boost::mutex> lock(mutex_);
        return id_++;
    };

private:
    boost::mutex mutex_;
    unsigned int id_;
};

}//end of namespace AMT

#endif // header
