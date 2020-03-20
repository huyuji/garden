#pragma once
#ifndef id5686556A_0A11_4B03_A9DF0C8A66A42B2E
#define id5686556A_0A11_4B03_A9DF0C8A66A42B2E

#include "car.h"
#include "task.h"
#include "job.h"
#include "carQualifier.h"

namespace AMT
{

class AMTService
{
public:
    /*!
     * Get singleton instance of AMTService.
     */
    static AMTService& getInstance();

    /*!
     * Register a car and provide its properties.
     * The Plate NO. of car is considered its unique identity when registering.
     * However, after the car is registered, its Plate NO. can be updated.
     *
     * @param[in]  properties   properties of the car.
     */
    virtual boost::shared_ptr<Car> registerCar(const CarProperties& properties) = 0;

    /*!
     * Get all the cars currently registered.
     */
    virtual std::vector<boost::shared_ptr<Car> > listCar() = 0;

    /*!
     * Find information about a car by Plate NO..
     * A CarException will be raised if the Plate NO. has not been not registered.
     *
     * @param[in]  plateNo  Plate No..
     */
    virtual boost::shared_ptr<Car> findCar(const PlateNo& plateNo) = 0;

    /*!
     * Update properties of a car.
     * Plate NO. can be updated by this function.
     * However, if the new Plate NO. has already been registered, a CarException will be raised.
     *
     * @param[in]  car          The car whose properties are to be update.
     * @param[in]  properties   New properties. 
     */
    virtual void updateCar(boost::shared_ptr<Car> car, const CarProperties& properties) = 0;

    /*!
     * Deregister a car.
     * All jobs currently registered for the car will be deleted as well.
     *
     * @param[in]  car  The car to delete.
     */
    virtual void deleteCar(boost::shared_ptr<Car> car) = 0;

    /*!
     * Register a task.
     * Task name must be unique. If the task name has already been registered, a TaskException will be raised.
     *
     * @param[in]  name         Task name.
     * @param[in]  description  Task description.
     * @param[in]  qualifier    A qualifier used to determine if the task can be performed on a car.
     */
    virtual boost::shared_ptr<Task> registerTask(
        const std::string& name, const std::string& description,
        boost::shared_ptr<CarQualifier> qualifier) = 0;

    /*!
     * Get all tasks currently registered
     */
    virtual std::vector<boost::shared_ptr<Task> > listTask() = 0;

    /*!
     * Find a task by its name.
     * A TaskException will be raised if the name has not been registered.
     *
     * @param[in]  name  Task name.
     */
    virtual boost::shared_ptr<Task> findTask(const std::string& name) = 0;

    /*!
     * Find all tasks that can be performed on a particular car.
     *
     * @param[in] car   The car to find suitable tasks for.
     */
    virtual std::vector<boost::shared_ptr<Task> > findTask(boost::shared_ptr<Car> car) = 0;

    /*!
     * Deregister a task.
     *
     * @param[in]  task     The task to delete.
     */
    virtual void deleteTask(boost::shared_ptr<Task> task) = 0;

    /*!
     * Register a job.
     * A CarQualifierException will be raised if the specified task cannot be performed for the car specified.
     *
     * @param[in]  car      The car to perform the job on.
     * @param[in]  task     The task to perform by the job.
     */
    virtual boost::shared_ptr<Job> registerJob(
        boost::shared_ptr<Car> car, boost::shared_ptr<Task> task) = 0;

    /*!
     * Get all jobs currently registered.
     */
    virtual std::vector<boost::shared_ptr<Job> > listJob() = 0;

    /*!
     * Get all jobs currently registered for a particular car.
     *
     * @param[in]  car  The car to find jobs for.
     */
    virtual std::vector<boost::shared_ptr<Job> > findJob(boost::shared_ptr<Car> car) = 0;

    /*!
     * Get the job of a particular task for a particular car.
     * A JobException will be raised if no such job.
     * 
     * @param[in]  car      The car to find job for.
     * @param[in]  task     The task of the job.
     */
    virtual boost::shared_ptr<Job> findJob(
        boost::shared_ptr<Car> car, boost::shared_ptr<Task> task) = 0;

    /*!
     * Add log for a job.
     * A JobException will be raised if the job has already been deleted.
     *
     * @param[in]  job          The job to add log for.
     * @param[in]  userName     Name of the user adding the log.
     * @param[in]  log          Content of the log.
     */
    virtual void updateJob(boost::shared_ptr<Job> job,
        const std::string& userName, const std::string& log) = 0;

    /*!
     * Update state of a job and add a log at the same time.
     * A JobException will be raised if the job has already been deleted.
     *
     * @param[in]  job          The job to add log for.
     * @param[in]  state        New state of the job.
     * @param[in]  userName     Name of the user updating job state.
     * @param[in]  log          Content of the log.
     */
    virtual void updateJob(boost::shared_ptr<Job> job, JobState state,
        const std::string& userName, const std::string& log) = 0;

    /*!
     * Delete a job.
     * 
     * @param[in]  job  The job to be deleted.
     */
    virtual void deleteJob(boost::shared_ptr<Job> job) = 0;

    virtual ~AMTService() {}

};

}//end of namespace AMT

#endif // header
