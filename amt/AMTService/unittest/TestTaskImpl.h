#include <boost/make_shared.hpp>

#include <gtest/gtest.h>

#include "taskImpl.h"

#include "CarMock.h"
#include "CarQualifierMock.h"

using namespace AMT;
using ::testing::_;

class TestTaskImpl : public ::testing::Test {
protected:
    virtual void SetUp() {
        qualifier.reset(new CarQualifierMock());

        tasks.push_back(boost::shared_ptr<Task>(new TaskImpl(
            "Battery Replacement", "replace battery", qualifier)));
        tasks.push_back(boost::shared_ptr<Task>(new TaskImpl(
            "Oil Change", "change oil", qualifier)));
        tasks.push_back(boost::shared_ptr<Task>(new TaskImpl(
            "Tire Rotation", "rotate tire", qualifier)));

        car = boost::make_shared<CarMock>();
    }

    std::vector<boost::shared_ptr<Task> > tasks;
    boost::shared_ptr< CarQualifierMock> qualifier;
    boost::shared_ptr<CarMock> car;
};

TEST_F(TestTaskImpl, getId)
{
    EXPECT_NE(tasks[0]->getId(), tasks[1]->getId());
}

TEST_F(TestTaskImpl, isQualified)
{
    EXPECT_CALL(*qualifier, isQualified(_)).Times(1);

    tasks[0]->isQualified(*car);
}

TEST_F(TestTaskImpl, verify)
{
    EXPECT_CALL(*qualifier, functionCall(_)).Times(1);

    tasks[0]->verify(*car);
}
