#include <gtest/gtest.h>

#include "job.h"
#include "CarMock.h"
#include "TaskMock.h"

using namespace AMT;
using ::testing::_;

class TestJob : public ::testing::Test {
protected:
    virtual void SetUp() {
        car.reset(new CarMock());
        task.reset(new TaskMock());
    }

    boost::shared_ptr<const CarMock> car;
    boost::shared_ptr<const TaskMock> task;
};

TEST_F(TestJob, ctor)
{
    EXPECT_CALL(*task, verify(_)).Times(1);

    //Job job(car, task);
}

TEST_F(TestJob, getId)
{
}

TEST_F(TestJob, isQualified)
{
}

TEST_F(TestJob, verify)
{
}
