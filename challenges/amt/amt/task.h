#pragma once
#ifndef id5399450E_C078_4258_9CBD21BF19A81909
#define id5399450E_C078_4258_9CBD21BF19A81909

#include <vector>
#include <time.h>
#include <boost/thread.hpp>
#include <boost/format.hpp>

#include "car.h"
#include "carQualifier.h"
#include "exception.h"
#include "idGenerator.h"

namespace AMT
{

class TaskException : public AMTException
{
public:
    explicit TaskException(const std::string& what) : AMTException(what)
    {
    };
};

class Task
{
public:
    Task(const std::string& name, const std::string& description,
         boost::shared_ptr<const CarQualifier> qualifier) : 
    id_(idGenerator_.next()), name_(name),
    description_(description), qualifier_(qualifier)
    {
    }

    unsigned int getId()                const { return id_; }
    const std::string& getName()        const { return name_; }
    const std::string& getDescription() const { return description_; }
    
    std::string toString()        const { return getDescription(); }

    bool isQualified(Car& car) const
    {
        return qualifier_->isQualified(car);
    }

    void verify(Car& car) const
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

private:
    Task(Task&);
    Task& operator = (Task&);

    static IdGenerator idGenerator_;

    const unsigned int id_;
    const std::string name_;
    const std::string description_;
    boost::shared_ptr<const CarQualifier> qualifier_;
};

bool operator == (const boost::shared_ptr<Task>& t1, const boost::shared_ptr<Task>& t2);

class TaskDepot
{
public:
    TaskDepot() {}

    boost::shared_ptr<Task> registerTask(const std::string& name, const std::string& description,
        boost::shared_ptr<const CarQualifier> qualifier)
    {
        WriteLock lock(mutex_);

        boost::shared_ptr<Task> task(new Task(name, description, qualifier));
        std::pair<TaskIter, bool> result = taskByName.insert(std::make_pair(name, task));

        if (result.second == false)
        {
            throw TaskException((boost::format(DUPLICATE_TASK) % name).str());
        }

        return result.first->second;
    };

    std::vector<boost::shared_ptr<Task> > listTask()
    {
        ReadLock lock(mutex_);

        std::vector<boost::shared_ptr<Task> > ret;
        for (TaskIter it = taskByName.begin(); it != taskByName.end(); ++it)
        {
            ret.push_back(it->second);
        }

        return ret;
    }

    boost::shared_ptr<Task> findTask(const std::string& name)
    {
        ReadLock lock(mutex_);

        CTaskIter it = taskByName.find(name);
        if (it == taskByName.end())
        {
            throw TaskException((boost::format(TASK_NOT_FOUND) % name).str());
        }

        return it->second;
    };

    std::vector<boost::shared_ptr<Task> > findTask(boost::shared_ptr<Car> car)
    {
        ReadLock lock(mutex_);

        std::vector<boost::shared_ptr<Task> > ret;
        for (CTaskIter it = taskByName.begin(); it != taskByName.end(); ++it)
        {
            if (it->second->isQualified(*car))
            {
                ret.push_back(it->second);
            }
        }

        return ret;
    }

    void deleteTask(boost::shared_ptr<Task> task)
    {
        WriteLock lock(mutex_);

        taskByName.erase(task->getName());
    }

private:
    TaskDepot(const TaskDepot&);
    TaskDepot& operator = (const TaskDepot&);

    typedef std::map<std::string, boost::shared_ptr<Task> > TaskMap;
    typedef TaskMap::iterator TaskIter;
    typedef TaskMap::const_iterator CTaskIter;
    TaskMap taskByName;

    typedef boost::unique_lock<boost::shared_mutex>  WriteLock;
    typedef boost::shared_lock<boost::shared_mutex>  ReadLock;
    boost::shared_mutex mutex_;
};

} //end of namespace AMT

#endif // header