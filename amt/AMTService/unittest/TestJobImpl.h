#include <boost/make_shared.hpp>

#include <gtest/gtest.h>

#include "jobImpl.h"

#include "CarMock.h"
#include "TaskMock.h"

using namespace AMT;
using ::testing::_;

class TestJobImpl : public ::testing::Test {
protected:
    virtual void SetUp() {
        car = boost::make_shared<CarMock>();
        task = boost::make_shared<TaskMock>();
    }

    boost::shared_ptr<CarMock> car;
    boost::shared_ptr<TaskMock> task;
};

TEST_F(TestJobImpl, getId)
{
    EXPECT_CALL(*task, verify(_)).Times(2);

    EXPECT_NE(JobImpl(car, task).getId(), JobImpl(car, task).getId());
}

TEST_F(TestJobImpl, addLog)
{
    EXPECT_CALL(*task, verify(_)).Times(1);

    JobImpl job(car, task);
    job.addLog(std::string(), std::string("event 1"));
    job.addLog(std::string(), std::string("event 2"));

    EXPECT_EQ(2, job.getLogs().size());
}

TEST_F(TestJobImpl, setState)
{
    EXPECT_CALL(*task, verify(_)).Times(1);

    JobImpl job(car, task);

    EXPECT_EQ(JobState::Open, job.getState());

    job.setState(JobState::InProgress, std::string(), std::string("start job"));
    EXPECT_EQ(JobState::InProgress, job.getState());
    EXPECT_EQ(1, job.getLogs().size());
}
