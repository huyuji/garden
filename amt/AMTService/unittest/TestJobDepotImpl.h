#include <gtest/gtest.h>

#include "jobDepotImpl.h"

#include "CarMock.h"
#include "TaskMock.h"
#include "JobMock.h"

using namespace AMT;
using ::testing::_;
using ::testing::ReturnRef;

class TestJobDepotImpl : public ::testing::Test {
protected:
    virtual void SetUp() {
        car1 = boost::make_shared<CarMock>();
        car2 = boost::make_shared<CarMock>();
        task1 = boost::make_shared<TaskMock>();
        task2 = boost::make_shared<TaskMock>();

        EXPECT_CALL(*car1, getId()).WillRepeatedly(Return(1));
        EXPECT_CALL(*car2, getId()).WillRepeatedly(Return(2));
        EXPECT_CALL(*task1, getId()).WillRepeatedly(Return(1));
        EXPECT_CALL(*task2, getId()).WillRepeatedly(Return(2));

        job1 = depot.registerJob(car1, task1);
        job2 = depot.registerJob(car1, task2);
        job3 = depot.registerJob(car2, task1);
        job4 = depot.registerJob(car2, task2);
    }

    std::string empty;
    boost::shared_ptr<CarMock> car1;
    boost::shared_ptr<CarMock> car2;
    boost::shared_ptr<TaskMock> task1;
    boost::shared_ptr<TaskMock> task2;

    boost::shared_ptr<Job> job1;
    boost::shared_ptr<Job> job2;
    boost::shared_ptr<Job> job3;
    boost::shared_ptr<Job> job4;

    JobDepotImpl depot;

};

TEST_F(TestJobDepotImpl, listJob)
{
    EXPECT_EQ(4, depot.listJob().size());
}

TEST_F(TestJobDepotImpl, findJobByCar)
{
    EXPECT_EQ(2, depot.findJob(car1).size());
}

TEST_F(TestJobDepotImpl, findJobByCarAndTask)
{
    EXPECT_EQ(job1, depot.findJob(car1, task1));
    EXPECT_EQ(job2, depot.findJob(car1, task2));
    EXPECT_EQ(job3, depot.findJob(car2, task1));
    EXPECT_EQ(job4, depot.findJob(car2, task2));
}

TEST_F(TestJobDepotImpl, deleteJob)
{
    std::string empty;
    EXPECT_CALL(*task1, getName()).WillRepeatedly(ReturnRef(empty));

    depot.deleteJob(job1);
    EXPECT_EQ(3, depot.listJob().size());
    EXPECT_THROW(depot.findJob(car1, task1), JobException);
}

TEST_F(TestJobDepotImpl, deleteJobByCar)
{
    EXPECT_CALL(*car1, getId()).WillRepeatedly(Return(1));

    depot.deleteJob(car1);
    EXPECT_EQ(2, depot.listJob().size());
    EXPECT_EQ(0, depot.findJob(car1).size());
}

TEST_F(TestJobDepotImpl, deleteJobByTask)
{
    depot.deleteJob(task2);
    EXPECT_EQ(2, depot.listJob().size());
}