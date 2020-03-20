#pragma once
#ifndef id321FCB9C_B2F9_4605_9F108AE6852CAB08
#define id321FCB9C_B2F9_4605_9F108AE6852CAB08

#include <time.h>

#include <boost/thread.hpp>

#include "job.h"
#include "idGenerator.h"

namespace AMT
{

class JobImpl : public Job
{
public:
    JobImpl(boost::shared_ptr<Car> car, boost::shared_ptr<Task> task)
        : task_(task), car_(car), isDeleted_(false), state_(JobState::Open)
    {
        task->verify(*car);
        id_ = idGenerator_.next();
        createTime_ = time(NULL);
    }

    virtual unsigned int getId() { return id_; }

    virtual boost::shared_ptr<Task> getTask() { return task_; }

    virtual boost::shared_ptr<Car> getCar() { return car_; }

    virtual time_t getCreateTime() { return createTime_; }

    virtual JobState getState();

    virtual std::vector<JobLogEntry> getLogs();

    virtual void addLog(const std::string& userName, const std::string& log);

    virtual void setState(
        JobState state, const std::string& userName, const std::string& log);

    virtual std::string toString();

    /**
     * Deleted job must be marked deleted to prevent further update.
     */
    void markDeleted();

private:
    JobImpl(const JobImpl&);
    JobImpl& operator = (const JobImpl&);

    static IdGenerator idGenerator_; /**< Used to generate unique ID. */

    /*!
     * constant fields
     */
    unsigned int id_;
    boost::shared_ptr<Task> task_;
    boost::shared_ptr<Car> car_;
    time_t createTime_;

    /*!
     * mutable fields
     */
    bool isDeleted_;
    JobState state_;
    std::vector<JobLogEntry> logs_;

    typedef boost::unique_lock<boost::shared_mutex>  WriteLock;
    typedef boost::shared_lock<boost::shared_mutex>  ReadLock;
    boost::shared_mutex mutex_; /**< Used to guard the access to mutable fields. */
};

bool operator == (const boost::shared_ptr<Job>& j1, const boost::shared_ptr<Job>& j2);

} //end of namespace AMT

#endif // header
