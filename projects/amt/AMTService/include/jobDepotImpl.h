#pragma once
#ifndef idA788257A_499C_4324_ACED084505220FFF
#define idA788257A_499C_4324_ACED084505220FFF

#include <map>

#include <boost/thread.hpp>

#include "jobDepot.h"

namespace AMT
{

class JobDepotImpl : public JobDepot
{
public:
    JobDepotImpl() {}

    virtual boost::shared_ptr<Job> registerJob(
        boost::shared_ptr<Car> car, boost::shared_ptr<Task> task);

    virtual std::vector<boost::shared_ptr<Job> > listJob();

    virtual std::vector<boost::shared_ptr<Job> > findJob(
        boost::shared_ptr<Car> car);

    virtual boost::shared_ptr<Job> findJob(
        boost::shared_ptr<Car> car, boost::shared_ptr<Task> task);

    virtual void deleteJob(boost::shared_ptr<Job> job);

    virtual void deleteJob(boost::shared_ptr<Car> car);

    virtual void deleteJob(boost::shared_ptr<Task> task);

private:
    JobDepotImpl(const JobDepotImpl&);
    JobDepotImpl& operator = (const JobDepotImpl&);

    typedef std::multimap<unsigned int, boost::shared_ptr<Job> > JobMap;
    typedef JobMap::iterator JobIter;
    typedef JobMap::const_iterator CJobIter;
    JobMap jobByCarId;
};

}//end of namespace AMT

#endif // header
