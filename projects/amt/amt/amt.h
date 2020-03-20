#pragma once
#ifndef id5686556A_0A11_4B03_A9DF0C8A66A42B2E
#define id5686556A_0A11_4B03_A9DF0C8A66A42B2E

#include <boost\ref.hpp>

#include "car.h"
#include "task.h"
#include "job.h"

namespace AMT
{

class AMTService
{
public:
    boost::shared_ptr<Car> registerCar(const CarProperties& properties)
    {
        return carDepot_.registerCar(properties);
    }

    std::vector<boost::shared_ptr<Car> > listCar()
    {
        return carDepot_.listCar();
    }

    boost::shared_ptr<Car> findCar(const PlateNo& plateNo)
    {
        return carDepot_.findCar(plateNo);
    }

    void updateCar(boost::shared_ptr<Car> car, const CarProperties& properties)
    {
        carDepot_.updateCar(car, properties);
    }

    void deleteCar(boost::shared_ptr<Car> car)
    {
        jobDepot_.deleteJob(car);
        carDepot_.deleteCar(car);
    }

    boost::shared_ptr<Task> registerTask(
        const std::string& name, const std::string& description,
        boost::shared_ptr<const CarQualifier> qualifier)
    {
        return taskDepot_.registerTask(name, description, qualifier);
    }

    std::vector<boost::shared_ptr<Task> > listTask()
    {
        return taskDepot_.listTask();
    }

    std::vector<boost::shared_ptr<Task> > findTask(boost::shared_ptr<Car> car)
    {
        return taskDepot_.findTask(car);
    }

    void deleteTask(boost::shared_ptr<Task> task)
    {
        jobDepot_.deleteJob(task);
        taskDepot_.deleteTask(task);
    }

    boost::shared_ptr<Job> registerJob(
        boost::shared_ptr<Car> car, boost::shared_ptr<Task> task)
    {
        jobDepot_.registerJob(car, task);
    }

    std::vector<boost::shared_ptr<Job> > listJob()
    {
        return jobDepot_.listJob();
    }

    std::vector<boost::shared_ptr<Job> > findJob(boost::shared_ptr<Car> car)
    {
        return jobDepot_.findJob(car);
    }

    boost::shared_ptr<Job> findJob(
        boost::shared_ptr<Car> car, boost::shared_ptr<Task> task)
    {
        return jobDepot_.findJob(car, task);
    }

    void updateJob(boost::shared_ptr<Job> job, JobState state, 
        const std::string& userName, const std::string& log)
    {
        job->setState(state, userName, log);
    }

    void deleteJob(boost::shared_ptr<Job> job)
    {
        jobDepot_.deleteJob(job);
    }

private:
    TaskDepot taskDepot_;
    CarDepot carDepot_;
    JobDepot jobDepot_;
};

}//end of namespace AMT

#endif // header
