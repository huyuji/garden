#include <boost/format.hpp>

#include "taskImpl.h"
#include "message.h"

namespace AMT
{

IdGenerator TaskImpl::idGenerator_;

bool TaskImpl::isQualified(Car& car)
{
    return qualifier_->isQualified(car);
}

void TaskImpl::verify(Car& car)
{
    try
    {
        (*qualifier_)(car);
    }
    catch (CarQualifierException& e)
    {
        std::string error(
            (boost::format(UNSUPPORTED_TASK_FOR_CAR) % description_ % car.toString()).str());
        error += std::string(e.what());

        throw TaskException(error);
    }
}

bool operator == (const boost::shared_ptr<Task>& t1, const boost::shared_ptr<Task>& t2)
{
    return t1->getId() == t2->getId();
}

}//end of namespace AMT