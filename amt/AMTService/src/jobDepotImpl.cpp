#include <boost/format.hpp>

#include "jobImpl.h"
#include "jobDepotImpl.h"
#include "message.h"

namespace AMT
{

boost::shared_ptr<Job> JobDepotImpl::registerJob(
    boost::shared_ptr<Car> car, boost::shared_ptr<Task> task)
{
    std::pair<CJobIter, CJobIter> result = jobByCarId.equal_range(car->getId());

    //verify the task has not already been registered for the car
    for (CJobIter it = result.first; it != result.second; ++it)
    {
        if (it->second->getTask() == task)
        {
            throw JobException(
                (boost::format(DUPLICATE_TASK_FOR_CAR) % task->getName() % car->toString()).str());
        }
    }

    boost::shared_ptr<Job> job(new JobImpl(car, task));

    jobByCarId.insert(std::make_pair(car->getId(), job));

    return job;
}

std::vector<boost::shared_ptr<Job> > JobDepotImpl::listJob()
{
    std::vector<boost::shared_ptr<Job> > ret;
    for (CJobIter it = jobByCarId.begin(); it != jobByCarId.end(); ++it)
    {
        ret.push_back(it->second);
    }

    return ret;
}

std::vector<boost::shared_ptr<Job> > JobDepotImpl::findJob(
    boost::shared_ptr<Car> car)
{
    std::pair<CJobIter, CJobIter> result = jobByCarId.equal_range(car->getId());
    std::vector<boost::shared_ptr<Job> > ret;
    for (CJobIter it = result.first; it != result.second; ++it)
    {
        ret.push_back(it->second);
    }

    return ret;
}

boost::shared_ptr<Job> JobDepotImpl::findJob(
    boost::shared_ptr<Car> car, boost::shared_ptr<Task> task)
{
    std::pair<CJobIter, CJobIter> result = jobByCarId.equal_range(car->getId());
    for (CJobIter it = result.first; it != result.second; ++it)
    {
        if (it->second->getTask() == task)
        {
            return it->second;
        }
    }

    throw JobException((boost::format(NO_SUCH_TASK_FOR_CAR) % task->getName() % car->toString()).str());
}

void JobDepotImpl::deleteJob(boost::shared_ptr<Job> job)
{
    std::pair<CJobIter, CJobIter> result = jobByCarId.equal_range(job->getCar()->getId());
    for (CJobIter it = result.first; it != result.second; ++it)
    {
        if (it->second == job)
        {
            jobByCarId.erase(it);
            boost::dynamic_pointer_cast<JobImpl>(job)->markDeleted();
            return;
        }
    }
}

void JobDepotImpl::deleteJob(boost::shared_ptr<Car> car)
{
    std::pair<CJobIter, CJobIter> result = jobByCarId.equal_range(car->getId());
    
    for (CJobIter it = result.first; it != result.second; ++it)
    {
        boost::dynamic_pointer_cast<JobImpl>(it->second)->markDeleted();
    }

    jobByCarId.erase(result.first, result.second);
}

void JobDepotImpl::deleteJob(boost::shared_ptr<Task> task)
{
    for (JobIter it = jobByCarId.begin(); it != jobByCarId.end();)
    {
        if (it->second->getTask() == task)
        {
            JobIter next = it;
            ++next;
            jobByCarId.erase(it);
            it = next;
        }
        else
        {
            ++it;
        }
    }
}

}//end of namespace AMT