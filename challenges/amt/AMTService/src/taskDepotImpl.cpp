#include <boost/format.hpp>

#include "taskImpl.h"
#include "taskDepotImpl.h"
#include "message.h"

namespace AMT
{

boost::shared_ptr<Task> TaskDepotImpl::registerTask(
    const std::string& name, const std::string& description,
    boost::shared_ptr<CarQualifier> qualifier)
{
    boost::shared_ptr<Task> task(new TaskImpl(name, description, qualifier));
    std::pair<TaskIter, bool> result = taskByName.insert(std::make_pair(name, task));

    //throw exception if task name has already been registered
    if (result.second == false)
    {
        throw TaskException((boost::format(DUPLICATE_TASK) % name).str());
    }

    return result.first->second;
};

std::vector<boost::shared_ptr<Task> > TaskDepotImpl::listTask()
{
    std::vector<boost::shared_ptr<Task> > ret;
    for (TaskIter it = taskByName.begin(); it != taskByName.end(); ++it)
    {
        ret.push_back(it->second);
    }

    return ret;
}

boost::shared_ptr<Task> TaskDepotImpl::findTask(const std::string& name)
{
    CTaskIter it = taskByName.find(name);
    if (it == taskByName.end())
    {
        throw TaskException((boost::format(TASK_NOT_FOUND) % name).str());
    }

    return it->second;
};

std::vector<boost::shared_ptr<Task> > TaskDepotImpl::findTask(
    boost::shared_ptr<Car> car)
{
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

void TaskDepotImpl::deleteTask(boost::shared_ptr<Task> task)
{
    taskByName.erase(task->getName());
}

}//end of namespace AMT