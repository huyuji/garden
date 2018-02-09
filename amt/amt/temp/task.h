#pragma once
#ifndef id5399450E_C078_4258_9CBD21BF19A81909
#define id5399450E_C078_4258_9CBD21BF19A81909

#include <vector>
#include <time.h>
#include <boost\thread.hpp>

#include "car.h"
#include "carQualifier.h"
#include "exception.h"
#include "idGenerator.h"

namespace AMT
{

enum TaskState
{
    Open = 0,
    InProgress,
    Complete,
    Dismissed,
};

class TaskException : public AMTException
{
public:
    explicit TaskException(const std::string& what) : AMTException(what)
    {
    };
};

class TaskLogEntry
{
public:
    TaskLogEntry(const std::string& userName, const std::string& log)
        : userName_(userName), log_(log), timestamp_(time(NULL))
    {};

    const std::string& getUserName() const { return userName_; };
    const std::string& getLog() const { return log_; };
    const time_t getTime() const { return timestamp_; };

private:
    const std::string userName_;
    const time_t timestamp_;
    const std::string log_;
};

class Task
{
public:
    Task(Car& car) 
    {
    };

    virtual const unsigned int getId() = 0;
    virtual const Car& getCar() = 0;
    virtual const TaskState getState() = 0;
    virtual const std::vector<TaskLogEntry> getLogs() = 0;
    virtual void setState(TaskState state, const std::string& userName, const std::string& log) = 0;
    virtual std::string getActionName() = 0;

    virtual ~Task()
    {
    };
};

template<class Qualifier>
class QualifiedTask : public Task
{
public:
    QualifiedTask(Car& car) : car_(car), state_(TaskState::Open)
    {
        Qualifier qualifier;
        try
        {
            qualifier(car);
        }
        catch (QualifierException& e)
        {
            std::string error("Cannot ");
            error += getActionName();
            error += " for ";
            error += car.toString();
            error += ". ";
            error += std::string(e.what());

            throw TaskException(error);
        }

        id_ = idGenerator_.next();
    };

    const unsigned int getId()  const { return id_; };
    Car& getCar()         const { return car_; };

    const TaskState getState()                  { ReadLock lock(mutex_); return state_; };
    const std::vector<TaskLogEntry> getLogs()   { ReadLock lock(mutex_); return logs_; };

    void setState(TaskState state, const std::string& userName, const std::string& log)
    {
        WriteLock lock(mutex_);
        state_ = state;
        logs_.push_back(TaskLogEntry(userName, log));
    };

private:
    typedef boost::unique_lock<boost::shared_mutex>  WriteLock;
    typedef boost::shared_lock<boost::shared_mutex>  ReadLock;

    static IdGenerator idGenerator_;

    unsigned int id_;
    Car& car_;
    TaskState state_;
    std::vector<TaskLogEntry> logs_;

    boost::shared_mutex mutex_;
};

class BatteryReplacement : public QualifiedTask<ElectricCarQualifier>
{
public:
    BatteryReplacement(Car& car) : QualifiedTask<ElectricCarQualifier>(car) {};

    std::string getActionName() { return std::string("replace battery"); };
};

class OilChange : public QualifiedTask<FuelCarQualifier>
{
public:
    OilChange(Car& car) : QualifiedTask<FuelCarQualifier>(car) {};

    std::string getActionName() { return std::string("change oil"); };
};

class TireRotation : public QualifiedTask<AnyCarQualifier>
{
public:
    TireRotation(Car& car) : QualifiedTask<AnyCarQualifier>(car) {};

    std::string getActionName() { return std::string("rotate tire"); };
};

} //end of namespace AMT

#endif // header