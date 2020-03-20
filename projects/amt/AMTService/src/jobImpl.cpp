#include <boost/format.hpp>

#include "jobImpl.h"
#include "message.h"

namespace AMT
{

IdGenerator JobImpl::idGenerator_;

JobState JobImpl::getState() 
{ 
    ReadLock lock(mutex_); 
    
    return state_; 
}

std::vector<JobLogEntry> JobImpl::getLogs() 
{ 
    ReadLock lock(mutex_); 
    
    return logs_; 
}

void JobImpl::addLog(const std::string& userName, const std::string& log)
{
    WriteLock lock(mutex_);
    
    if (isDeleted_)
    {
        throw JobException((boost::format(JOB_DELETED) % toString()).str());
    }

    logs_.push_back(JobLogEntry(userName, log));
}

void JobImpl::setState(
    JobState state, const std::string& userName, const std::string& log)
{
    WriteLock lock(mutex_);

    if (isDeleted_)
    {
        throw JobException((boost::format(JOB_DELETED) % toString()).str());
    }

    state_ = state;
    logs_.push_back(JobLogEntry(userName, log));
}

std::string JobImpl::toString()
{
    return (boost::format(TASK_FOR_CAR) % task_->toString() % car_->toString()).str();
}

void JobImpl::markDeleted()
{
    WriteLock lock(mutex_);
    
    isDeleted_ = true;
}

bool operator == (const boost::shared_ptr<Job>& j1, const boost::shared_ptr<Job>& j2)
{
    return j1->getId() == j2->getId();
}

}//end of namespace AMT