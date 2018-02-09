#include <iostream>

#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

#include <gtest/gtest.h>

#include "amtImpl.h"

using namespace AMT;

namespace
{
    PlateNo generatePlate()
    {
        static unsigned int count_ = 1;
        static boost::mutex mutex_;

        boost::unique_lock<boost::mutex> lock(mutex_);
        count_++;
        return "Plate" + boost::lexical_cast<std::string>(count_);
    }

    CarProperties FordInfo(generatePlate(), Make("Ford"), Model("M"), Power::Diesel, 2011, 10000);
    CarProperties ToyotoInfo(generatePlate(), Make("Toyoto"), Model("C"), Power::Gas, 2011, 10000);
    CarProperties TeslaInfo(generatePlate(), Make("Tesla"), Model("T"), Power::Electric, 2011, 10000);
    CarProperties ChevronInfo(generatePlate(), Make("Chevron"), Model("A"), Power::Hybrid, 2011, 10000);
}

class TestAMTServiceImpl : public ::testing::Test {
protected:
    TestAMTServiceImpl() : svc(AMTService::getInstance()) 
    {
    }
    
    AMTService& svc;
};

TEST_F(TestAMTServiceImpl, registerAndListTask)
{
    using namespace boost;

    std::cout << "Register 3 tasks.\n";

    svc.registerTask("Battery Replacement", "replace battery", boost::make_shared<ElectricCarQualifier>());
    svc.registerTask("Oil Change", "change oil", boost::make_shared<FuelCarQualifier>());
    svc.registerTask("Tire Rotation", "rotate tire", boost::make_shared<AnyCarQualifier>());
    EXPECT_EQ(3, svc.listTask().size());

    std::cout << "Registered Tasks:\n";
    std::vector<boost::shared_ptr<Task> > tasks = svc.listTask();
    BOOST_FOREACH(boost::shared_ptr<Task> task, tasks)
    {
        std::cout << task->toString() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Register a task with the same name as Battery Replacement.\n";
    try
    {
        svc.registerTask("Battery Replacement", "replace common battery", boost::make_shared<AnyCarQualifier>());
    }
    catch (AMTException& e)
    {
        std::cout << "Failed to register task with duplicate name. " << e.what() << std::endl;
    }
    std::cout << std::endl;
    EXPECT_THROW(svc.registerTask("Battery Replacement", "replace common battery", boost::make_shared<AnyCarQualifier>()),
        TaskException);
}

TEST_F(TestAMTServiceImpl, findTaskByName)
{
    std::cout << "Find task Oil Change by name.\n";

    boost::shared_ptr<Task> task = svc.findTask("Oil Change");

    std::cout << "Try to find an unregistered task name: Transform.\n";

    try
    {
        svc.findTask("Transform");
    }
    catch (AMTException& e)
    {
        std::cout << "Task not found. " << e.what() << std::endl;
    }
    std::cout << std::endl;
    EXPECT_THROW(svc.findTask("Transform"), TaskException);
}

TEST_F(TestAMTServiceImpl, registerAndListCar)
{
    std::cout << "Register 4 cars.\n";
    boost::shared_ptr<Car> Ford = svc.registerCar(FordInfo);
    boost::shared_ptr<Car> Toyoto = svc.registerCar(ToyotoInfo);
    boost::shared_ptr<Car> Tesla = svc.registerCar(TeslaInfo);
    boost::shared_ptr<Car> Chevron = svc.registerCar(ChevronInfo);
    EXPECT_EQ(4, svc.listCar().size());

    std::cout << "Registered Cars:\n";
    std::vector<boost::shared_ptr<Car> > cars = svc.listCar();
    BOOST_FOREACH(boost::shared_ptr<Car> car, cars)
    {
        std::cout << car->toString() << std::endl;
    }
    std::cout << std::endl;
}

TEST_F(TestAMTServiceImpl, findCarByPlateNo)
{
    std::cout << "Find car with plate No. " << FordInfo.getPlateNo() << ".\n";
    boost::shared_ptr<Car> car = svc.findCar(FordInfo.getPlateNo());

    std::cout << "The car with Plate NO. " << FordInfo.getPlateNo() 
        << " is a " << car->toString() << std::endl;
    std::cout << std::endl;

    EXPECT_EQ(car->getProperties(), FordInfo);
}

TEST_F(TestAMTServiceImpl, tryToFindNotRegisteredCar)
{
    PlateNo newPlateNo = generatePlate();

    std::cout << "Try to find unregistered plate No. " << newPlateNo << ".\n";

    try
    {
        svc.findCar(newPlateNo);
    }
    catch (AMTException& e)
    {
        std::cout << "Plate NO. " << newPlateNo << " not registered. " << e.what() << std::endl;
    }
    std::cout << std::endl;
    EXPECT_THROW(svc.findCar(newPlateNo), CarException);
}

TEST_F(TestAMTServiceImpl, updateCar)
{
    std::cout << "Update mileage of Ford to 20000.\n";
    boost::shared_ptr<Car> car = svc.findCar(FordInfo.getPlateNo());
    CarProperties prop = car->getProperties();
    prop.setMileage(20000);
    svc.updateCar(car, prop);
    EXPECT_EQ(prop, svc.findCar(FordInfo.getPlateNo())->getProperties());

    prop.setPlateNo(generatePlate());
    std::cout << "Update plate No of Ford to " << prop.getPlateNo() << ".\n";
    svc.updateCar(car, prop);
    FordInfo.setPlateNo(prop.getPlateNo());
    EXPECT_EQ(prop, svc.findCar(FordInfo.getPlateNo())->getProperties());

    std::cout << "Try to update plate No of Ford to that of Tesla.\n";
    prop.setPlateNo(TeslaInfo.getPlateNo());
    try
    {
        svc.updateCar(car, prop);
    }
    catch (AMTException& e)
    {
        std::cout << "Failed to update plate NO. " << e.what() << std::endl;
    }
    std::cout << std::endl;
    EXPECT_THROW(svc.updateCar(car, prop), CarException);
}

TEST_F(TestAMTServiceImpl, findTaskForCar)
{
    std::cout << "Find all tasks supported for Ford.\n";
    boost::shared_ptr<Car> car = svc.findCar(FordInfo.getPlateNo());
    std::vector<boost::shared_ptr<Task> > tasks = svc.findTask(car);

    std::cout << "Tasks supported for the " << car->toString() << ":\n";
    using namespace boost;
    BOOST_FOREACH(boost::shared_ptr<Task> task, tasks)
    {
        std::cout << task->toString() << std::endl;
    }
    std::cout << std::endl;
}

TEST_F(TestAMTServiceImpl, registerAndListJob)
{
    boost::shared_ptr<Car> ford = svc.findCar(FordInfo.getPlateNo());
    std::vector<boost::shared_ptr<Task> > tasks = svc.findTask(ford);

    unsigned int count = 0;
    for (unsigned int i = 0; i < tasks.size(); i++)
    {
        svc.registerJob(ford, tasks[i]);
        count++;
    }
    std::cout << "Register " << count << "job(s) for Ford.\n";
    EXPECT_EQ(count, svc.listJob().size());

    std::cout << "Current jobs:\n";
    std::vector<boost::shared_ptr<Job> > jobs = svc.listJob();
    using namespace boost;
    BOOST_FOREACH(boost::shared_ptr<Job> job, jobs)
    {
        std::cout << job->toString() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Try to register a task with existing job for Ford.\n";
    try
    {
        svc.registerJob(ford, tasks[0]);
    }
    catch (AMTException& e)
    {
        std::cout << "Failed to register duplicate job. " << e.what() << std::endl;
    }
    std::cout << std::endl;
    EXPECT_THROW(svc.registerJob(ford, tasks[0]), JobException);
}

TEST_F(TestAMTServiceImpl, findJobByCar)
{
    std::cout << "Find all jobs for Ford.\n";

    boost::shared_ptr<Car> ford = svc.findCar(FordInfo.getPlateNo());

    std::vector<boost::shared_ptr<Job> > jobs = svc.findJob(ford);
    std::cout << "Jobs for the " << ford->toString() << ":\n";
    BOOST_FOREACH(boost::shared_ptr<Job> job, jobs)
    {
        std::cout << job->getTask()->toString() << std::endl;
    }
    std::cout << std::endl;
    EXPECT_EQ(jobs.size(), svc.findTask(ford).size());
}

TEST_F(TestAMTServiceImpl, findJobByCarAndTask)
{
    std::cout << "Find all jobs for Ford in another way.\n";

    boost::shared_ptr<Car> ford = svc.findCar(FordInfo.getPlateNo());
    std::vector<boost::shared_ptr<Task> > tasks = svc.listTask();

    std::cout << "Jobs for the " << ford->toString() << ":\n";
    unsigned int count = 0;
    using namespace boost;
    BOOST_FOREACH(boost::shared_ptr<Task> task, tasks)
    {
        try
        {
            svc.findJob(ford, task);
            count++;
            std::cout << task->toString() << std::endl;
        }
        catch (JobException&)
        {            
        }
    }
    std::cout << std::endl;
    EXPECT_EQ(count, svc.findTask(ford).size());
}

TEST_F(TestAMTServiceImpl, updateJob)
{
    boost::shared_ptr<Car> ford = svc.findCar(FordInfo.getPlateNo());

    std::vector<boost::shared_ptr<Job> > jobs = svc.findJob(ford);

    std::cout << "Joe has started and then completed a job.\n";
    svc.updateJob(jobs[0], JobState::InProgress, "Joe", "Joe has taken the job");
    svc.updateJob(jobs[0], JobState::Complete, "Joe", "Joe has completed the job");
    EXPECT_EQ(2, jobs[0]->getLogs().size());
   
    std::cout << "Work log of " << jobs[0]->toString() << ":\n";
    std::vector<JobLogEntry> logs = jobs[0]->getLogs();
    using namespace boost;
    BOOST_FOREACH(const JobLogEntry& log, logs)
    {
        time_t t = log.getTime();
        std::cout << asctime(gmtime(&t)) << "\t" << log.getUserName() << " : " << log.getLog() << std::endl;
    }
    std::cout << std::endl;
}

TEST_F(TestAMTServiceImpl, deleteJob)
{
    boost::shared_ptr<Car> ford = svc.findCar(FordInfo.getPlateNo());

    std::vector<boost::shared_ptr<Job> > jobs = svc.findJob(ford);

    std::cout << "The job: " << jobs[1]->toString() << " is dismissed by Joe.\n";
    svc.updateJob(jobs[1], JobState::Dismissed, "Joe", "Customer will DIY the task");

    std::cout << "Delete dismissed jobs for the " << ford->toString() << ":\n";
    using namespace boost;
    BOOST_FOREACH(boost::shared_ptr<Job> job, jobs)
    {
        if (job->getState() == JobState::Dismissed)
        {
            std::cout << job->getTask()->toString() << std::endl;
            svc.deleteJob(job);
        }
    }
    std::cout << std::endl;
}

TEST_F(TestAMTServiceImpl, deleteCar)
{
    boost::shared_ptr<Car> ford = svc.findCar(FordInfo.getPlateNo());
    std::cout << "Delete the " << ford->toString() << ".\n";
    svc.deleteCar(ford);

    std::vector<boost::shared_ptr<Job> > jobs = svc.findJob(ford);
    std::cout << "There are " << jobs.size() << " job for the " << ford->toString() << " now.\n";
    std::cout << std::endl;
}

TEST_F(TestAMTServiceImpl, deleteTask)
{
    boost::shared_ptr<Task> task = svc.findTask("Tire Rotation");

    std::vector<boost::shared_ptr<Car> > cars = svc.listCar();

    std::cout << "Register some jobs.\n";
    using namespace boost;
    BOOST_FOREACH(boost::shared_ptr<Car> car, cars)
    {
        if (task->isQualified(*car))
        {
            boost::shared_ptr<Job> job = svc.registerJob(car, task);
            std::cout << "Job registered: " << job->toString() << std::endl;
        }
    }
    std::cout << std::endl;
    EXPECT_EQ(cars.size(), svc.listJob().size());

    std::cout << "We no longer do the Tire Rotation business. Remove it.\n";
    svc.deleteTask(task);
    
    std::cout << "There are " << svc.listJob().size() << " job now.\n";
}