#include <boost/thread/thread.hpp>
#include <boost/thread/once.hpp>
#include <boost/make_shared.hpp>

#include "amtImpl.h"
#include "taskDepotImpl.h"
#include "carDepotImpl.h"
#include "jobDepotImpl.h"

using namespace AMT;

namespace
{
    boost::shared_ptr<AMTServiceImpl> defaultService;
    boost::once_flag once = BOOST_ONCE_INIT;

    void init()
    {
        defaultService = boost::make_shared<AMTServiceImpl>(
            boost::make_shared<TaskDepotImpl>(),
            boost::make_shared<CarDepotImpl>(),
            boost::make_shared<JobDepotImpl>());
    }
}

AMTService& AMTService::getInstance()
{
    boost::call_once(&init, once);

    return *defaultService;
}