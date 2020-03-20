#pragma once
#ifndef idD74AB902_6D6A_4260_84A0E6E49D02F42A
#define idD74AB902_6D6A_4260_84A0E6E49D02F42A

#include "job.h"

namespace AMT
{

class JobDepot
{
public:
    virtual boost::shared_ptr<Job> registerJob(
        boost::shared_ptr<Car> car, boost::shared_ptr<Task> task) = 0;

    virtual std::vector<boost::shared_ptr<Job> > listJob() = 0;

    virtual std::vector<boost::shared_ptr<Job> > findJob(
        boost::shared_ptr<Car> car) = 0;

    virtual boost::shared_ptr<Job> findJob(
        boost::shared_ptr<Car> car, boost::shared_ptr<Task> task) = 0;

    virtual void deleteJob(boost::shared_ptr<Job> job) = 0;

    virtual void deleteJob(boost::shared_ptr<Car> car) = 0;

    virtual void deleteJob(boost::shared_ptr<Task> task) = 0;

    virtual ~JobDepot() {}
};

}//end of namespace AMT

#endif // header
