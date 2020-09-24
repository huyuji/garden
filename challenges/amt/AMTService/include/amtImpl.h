#pragma once
#ifndef idDE7127E7_30CB_4EFE_914E24726EA27FEB
#define idDE7127E7_30CB_4EFE_914E24726EA27FEB

#include "amt.h"
#include "taskDepot.h"
#include "jobDepot.h"
#include "carDepot.h"

namespace AMT
{

class AMTServiceImpl : public AMTService
{
public:
    AMTServiceImpl(
        boost::shared_ptr<TaskDepot> taskDepot,
        boost::shared_ptr<CarDepot> carDepot,
        boost::shared_ptr<JobDepot> jobDepot)
        : taskDepot_(taskDepot), carDepot_(carDepot), jobDepot_(jobDepot)
    {
    }

    virtual boost::shared_ptr<Car> registerCar(const CarProperties& properties)
    {
        WriteLock lock(carMutex_);
        
        return carDepot_->registerCar(properties);
    }

    virtual std::vector<boost::shared_ptr<Car> > listCar()
    {
        ReadLock lock(carMutex_);
        
        return carDepot_->listCar();
    }

    virtual boost::shared_ptr<Car> findCar(const PlateNo& plateNo)
    {
        ReadLock lock(carMutex_);
        
        return carDepot_->findCar(plateNo);
    }

    virtual void updateCar(boost::shared_ptr<Car> car, const CarProperties& properties)
    {
        WriteLock lock(carMutex_);
        
        carDepot_->updateCar(car, properties);
    }

    virtual void deleteCar(boost::shared_ptr<Car> car)
    {
        WriteLock lockCar(carMutex_);
        WriteLock lockJob(jobMutex_);
        
        jobDepot_->deleteJob(car);
        carDepot_->deleteCar(car);
    }

    virtual boost::shared_ptr<Task> registerTask(
        const std::string& name, const std::string& description,
        boost::shared_ptr< CarQualifier> qualifier)
    {
        WriteLock lock(taskMutex_);
        
        return taskDepot_->registerTask(name, description, qualifier);
    }

    virtual std::vector<boost::shared_ptr<Task> > listTask()
    {
        ReadLock lock(taskMutex_);
        
        return taskDepot_->listTask();
    }

    virtual boost::shared_ptr<Task> findTask(const std::string& name)
    {
        ReadLock lock(taskMutex_);
        
        return taskDepot_->findTask(name);
    }

    virtual std::vector<boost::shared_ptr<Task> > findTask(boost::shared_ptr<Car> car)
    {
        ReadLock lock(taskMutex_);
        
        return taskDepot_->findTask(car);
    }

    virtual void deleteTask(boost::shared_ptr<Task> task)
    {
        WriteLock lockJob(jobMutex_);
        WriteLock lockTask(taskMutex_);
        
        jobDepot_->deleteJob(task);
        taskDepot_->deleteTask(task);
    }

    virtual boost::shared_ptr<Job> registerJob(
        boost::shared_ptr<Car> car, boost::shared_ptr<Task> task)
    {
        WriteLock lock(jobMutex_);
        
        return jobDepot_->registerJob(car, task);
    }

    virtual std::vector<boost::shared_ptr<Job> > listJob()
    {
        ReadLock lock(jobMutex_);
        
        return jobDepot_->listJob();
    }

    virtual std::vector<boost::shared_ptr<Job> > findJob(boost::shared_ptr<Car> car)
    {
        ReadLock lock(jobMutex_);
        
        return jobDepot_->findJob(car);
    }

    virtual boost::shared_ptr<Job> findJob(
        boost::shared_ptr<Car> car, boost::shared_ptr<Task> task)
    {
        ReadLock lock(jobMutex_);
        
        return jobDepot_->findJob(car, task);
    }

    virtual void updateJob(boost::shared_ptr<Job> job,
        const std::string& userName, const std::string& log)
    {
        job->addLog(userName, log);
    }

    virtual void updateJob(boost::shared_ptr<Job> job, JobState state,
        const std::string& userName, const std::string& log)
    {
        job->setState(state, userName, log);
    }

    virtual void deleteJob(boost::shared_ptr<Job> job)
    {
        WriteLock lock(jobMutex_);
        
        jobDepot_->deleteJob(job);
    }

private:
    typedef boost::unique_lock<boost::shared_mutex>  WriteLock;
    typedef boost::shared_lock<boost::shared_mutex>  ReadLock;

    /*!
     * When multiple depots must be lock, 
     * acquire carMutex_, jobMutex_, taskMutex_ in strict (alphabetic, if it helps) order to avoid deadlock.
     */
    boost::shared_ptr<CarDepot> carDepot_;
    boost::shared_mutex carMutex_;

    boost::shared_ptr<JobDepot> jobDepot_;
    boost::shared_mutex jobMutex_;

    boost::shared_ptr<TaskDepot> taskDepot_;
    boost::shared_mutex taskMutex_;
};

}//end of namespace AMT

#endif // header
