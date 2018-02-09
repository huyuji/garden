#pragma once
#ifndef idEAEBED8C_A291_4E1E_9F0558A8246642EC
#define idEAEBED8C_A291_4E1E_9F0558A8246642EC

#include <vector>

#include <boost/shared_ptr.hpp>

#include "task.h"
#include "carQualifier.h"

namespace AMT
{

class TaskDepot
{
public:
    virtual boost::shared_ptr<Task> registerTask(
        const std::string& name, const std::string& description,
        boost::shared_ptr<CarQualifier> qualifier) = 0;

    virtual std::vector<boost::shared_ptr<Task> > listTask() = 0;

    virtual boost::shared_ptr<Task> findTask(const std::string& name) = 0;

    virtual std::vector<boost::shared_ptr<Task> > findTask(boost::shared_ptr<Car> car) = 0;

    virtual void deleteTask(boost::shared_ptr<Task> task) = 0;

    virtual ~TaskDepot() {};
};

}//end of namespace AMT

#endif // header
