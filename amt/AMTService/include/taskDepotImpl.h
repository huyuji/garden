#pragma once
#ifndef id1E64EE69_F2D7_4742_9C3815D19EF9BDC9
#define id1E64EE69_F2D7_4742_9C3815D19EF9BDC9

#include <map>

#include <boost/thread.hpp>

#include "taskDepot.h"

namespace AMT
{

class TaskDepotImpl : public TaskDepot
{
public:
    TaskDepotImpl() {}

    virtual boost::shared_ptr<Task> registerTask(
        const std::string& name, const std::string& description,
        boost::shared_ptr<CarQualifier> qualifier);

    virtual std::vector<boost::shared_ptr<Task> > listTask();

    virtual boost::shared_ptr<Task> findTask(const std::string& name);

    virtual std::vector<boost::shared_ptr<Task> > findTask(boost::shared_ptr<Car> car);

    virtual void deleteTask(boost::shared_ptr<Task> task);

private:
    TaskDepotImpl(const TaskDepot&);
    TaskDepot& operator = (const TaskDepot&);

    typedef std::map<std::string, boost::shared_ptr<Task> > TaskMap;
    typedef TaskMap::iterator TaskIter;
    typedef TaskMap::const_iterator CTaskIter;
    TaskMap taskByName;
};

}//end of namespace AMT

#endif // header
