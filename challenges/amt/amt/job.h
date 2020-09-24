#pragma once
#ifndef idB81B24A9_3234_45FA_83A13001C5B2E6E2
#define idB81B24A9_3234_45FA_83A13001C5B2E6E2

#include <string>
#include <time.h>
#include <map>
#include <boost\thread.hpp>
#include <boost\shared_ptr.hpp>

#include "task.h"
#include "car.h"

namespace AMT
{

class JobException : public AMTException
{
public:
    explicit JobException(const std::string& what) : AMTException(what)
    {
    };
};

enum JobState
{
    Open = 0,
    InProgress,
    Complete,
    Dismissed,
};

class JobLogEntry
{
public:
    JobLogEntry(const std::string& userName, const std::string& log)
        : userName_(userName), log_(log), timestamp_(time(NULL))
    {};

    const std::string& getUserName() const { return userName_; };
    const std::string& getLog()      const { return log_; };
    time_t getTime()                 const { return timestamp_; };

private:
    const std::string userName_;
    const time_t timestamp_;
    const std::string log_;
};

class Job
{
public:
    Job(boost::shared_ptr<Car> car, boost::shared_ptr<Task> task)
        : task_(task), car_(car), state_(JobState::Open)
    {
        task->verify(*car);
        id_ = idGenerator_.next();
        createTime_ = time(NULL);
    }

    unsigned int getId() const { return id_; }
    boost::shared_ptr<Task> getTask() const { return task_; }
    boost::shared_ptr<Car> getCar() const { return car_; }
    time_t getCreateTime() const { return createTime_; }

    JobState                 getState()  { ReadLock lock(mutex_); return state_; }
    std::vector<JobLogEntry> getLogs()   { ReadLock lock(mutex_); return logs_; }

    void addLog(const std::string& userName, const std::string& log)
    { 
        WriteLock lock(mutex_);
        logs_.push_back(JobLogEntry(userName, log));
    }

    void setState(JobState state, const std::string& userName, const std::string& log)
    {
        WriteLock lock(mutex_);
        state_ = state;
        logs_.push_back(JobLogEntry(userName, log));
    }

    std::string toString() 
    { 
        return (boost::format(TASK_FOR_CAR) % task_->toString() % car_->toString()).str(); 
    }

private:
    Job(const Job&);
    Job& operator = (const Job&);

    static IdGenerator idGenerator_;

    unsigned int id_;
    boost::shared_ptr<Task> task_;
    boost::shared_ptr<Car> car_;
    JobState state_;
    time_t createTime_;
    std::vector<JobLogEntry> logs_;

    typedef boost::unique_lock<boost::shared_mutex>  WriteLock;
    typedef boost::shared_lock<boost::shared_mutex>  ReadLock;
    boost::shared_mutex mutex_;
};

bool operator == (const boost::shared_ptr<Job>& j1, const boost::shared_ptr<Job>& j2);

class JobDepot
{
public:
    JobDepot() {}

    boost::shared_ptr<Job> registerJob(
        boost::shared_ptr<Car> car, boost::shared_ptr<Task> task)
    {
        boost::upgrade_lock<boost::shared_mutex> lock(mutex_);

        std::pair<CJobIter, CJobIter> result = jobByCarId.equal_range(car->getId());

        for (CJobIter it = result.first; it != result.second; ++it)
        {
            if (it->second->getTask() == task)
            {
                throw JobException(
                    (boost::format(DUPLICATE_TASK_FOR_CAR) % task->getName() % car->toString()).str());
            }
        }

        boost::shared_ptr<Job> job(new Job(car, task));

        boost::upgrade_to_unique_lock<boost::shared_mutex> exclusiveLock(lock);
        jobByCarId.insert(std::make_pair(car->getId(), job));

        return job;
    }

    std::vector<boost::shared_ptr<Job> > listJob()
    {
        ReadLock lock(mutex_);

        std::vector<boost::shared_ptr<Job> > ret;
        for (CJobIter it = jobByCarId.begin(); it != jobByCarId.end(); ++it)
        {
            ret.push_back(it->second);
        }
    }

    std::vector<boost::shared_ptr<Job> > findJob(boost::shared_ptr<Car> car)
    {
        ReadLock lock(mutex_);

        std::pair<CJobIter, CJobIter> result = jobByCarId.equal_range(car->getId());
        std::vector<boost::shared_ptr<Job> > ret;
        for (CJobIter it = result.first; it != result.second; ++it)
        {
            ret.push_back(it->second);
        }

        return ret;
    }

    boost::shared_ptr<Job> findJob(boost::shared_ptr<Car> car, boost::shared_ptr<Task> task)
    {
        ReadLock lock(mutex_);

        std::pair<CJobIter, CJobIter> result = jobByCarId.equal_range(car->getId());
        for (CJobIter it = result.first; it != result.second; ++it)
        {
            if (it->second->getTask() == task)
            {
                return it->second;
            }
        }

        return boost::shared_ptr<Job>();
    }

    void deleteJob(boost::shared_ptr<Job> job)
    {
        WriteLock lock(mutex_);

        std::pair<CJobIter, CJobIter> result = jobByCarId.equal_range(job->getCar()->getId());
        for (CJobIter it = result.first; it != result.second; ++it)
        {
            if (it->second == job)
            {
                jobByCarId.erase(it);
                return;
            }
        }
    }

    void deleteJob(boost::shared_ptr<Car> car)
    {
        WriteLock lock(mutex_);

        std::pair<CJobIter, CJobIter> result = jobByCarId.equal_range(car->getId());
        for (CJobIter it = result.first; it != result.second; ++it)
        {
            jobByCarId.erase(it);
        }
    }

    void deleteJob(boost::shared_ptr<Task> task)
    {
        WriteLock lock(mutex_);

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

private:
    JobDepot(const JobDepot&);
    JobDepot& operator = (const JobDepot&);

    typedef std::multimap<unsigned int, boost::shared_ptr<Job> > JobMap;
    typedef JobMap::iterator JobIter;
    typedef JobMap::const_iterator CJobIter;
    JobMap jobByCarId;

    typedef boost::unique_lock<boost::shared_mutex>  WriteLock;
    typedef boost::shared_lock<boost::shared_mutex>  ReadLock;
    boost::shared_mutex mutex_;
};

} //end of namespace AMT

#endif // header
