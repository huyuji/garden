#pragma once
#ifndef idB81B24A9_3234_45FA_83A13001C5B2E6E2
#define idB81B24A9_3234_45FA_83A13001C5B2E6E2

#include <vector>

#include <boost/shared_ptr.hpp>

#include "task.h"

namespace AMT
{

class JobException : public AMTException
{
public:
    explicit JobException(const std::string& what) : AMTException(what)
    {
    }
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
    {
    }

    JobLogEntry(const JobLogEntry& j) 
        : userName_(j.userName_), log_(j.log_), timestamp_(j.timestamp_)
    {
    }

    const JobLogEntry& operator=(const JobLogEntry& j)
    {
        userName_ = j.userName_;
        timestamp_ = j.timestamp_;
        log_ = j.log_;

        return *this;
    }

    const std::string& getUserName() const { return userName_; }
    const std::string& getLog()      const { return log_; }
    time_t getTime()                 const { return timestamp_; }

private:
    std::string userName_;
    time_t timestamp_;
    std::string log_;
};

/*!
 * This class represents a job, which is a task to be performed for a particular car
 */
class Job
{
public:
    /*!
     * Get the unique ID of a job.
     */
    virtual unsigned int getId() = 0;

    virtual boost::shared_ptr<Task> getTask() = 0;

    virtual boost::shared_ptr<Car> getCar() = 0;

    virtual time_t getCreateTime() = 0;

    virtual JobState getState() = 0;

    virtual std::vector<JobLogEntry> getLogs() = 0;

    virtual void addLog(const std::string& userName, const std::string& log) = 0;

    virtual void setState(JobState state, const std::string& userName, const std::string& log) = 0;

    virtual std::string toString() = 0;

    virtual ~Job() {}
};

} //end of namespace AMT

#endif // header
