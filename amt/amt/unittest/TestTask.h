#include <gtest/gtest.h>

#include "..\task.h"
#include "CarMock.h"
#include "CarQualifierMock.h"

using namespace AMT;
using ::testing::_;

class TestTask : public ::testing::Test {
protected:
    virtual void SetUp() {
        qualifier.reset(new CarQualifierMock());
        tasks.push_back(boost::shared_ptr<Task>(new Task(
            "Battery Replacement", "replace battery", qualifier)));
        tasks.push_back(boost::shared_ptr<Task>(new Task(
            "Oil Change", "change oil", qualifier)));
        tasks.push_back(boost::shared_ptr<Task>(new Task(
            "Tire Rotation", "rotate tire", qualifier)));
    }

    std::vector<boost::shared_ptr<Task> > tasks;
    boost::shared_ptr<const CarQualifierMock> qualifier;
    CarMock car;
};

TEST_F(TestTask, getId)
{
    EXPECT_NE(tasks[0]->getId(), tasks[1]->getId());
}

TEST_F(TestTask, isQualified)
{
    EXPECT_CALL(*qualifier, isQualified(_)).Times(1);

    tasks[0]->isQualified(car);
}

TEST_F(TestTask, verify)
{
    EXPECT_CALL(*qualifier, functionCall(_)).Times(1);

    tasks[0]->verify(car);
}
