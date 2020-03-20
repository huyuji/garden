#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>

#include <gtest/gtest.h>

#include "taskDepotImpl.h"

#include "CarMock.h"
#include "CarQualifierMock.h"

using namespace AMT;
using ::testing::_;
using ::testing::Return;

class TestTaskDepotImpl : public ::testing::Test {
protected:
    virtual void SetUp() {
        qualifier.reset(new CarQualifierMock());
        car = boost::make_shared<CarMock>();

        tasks.push_back(boost::shared_ptr<Task>(new TaskImpl(
            "Battery Replacement", "replace battery", qualifier)));
        tasks.push_back(boost::shared_ptr<Task>(new TaskImpl(
            "Oil Change", "change oil", qualifier)));
        tasks.push_back(boost::shared_ptr<Task>(new TaskImpl(
            "Tire Rotation", "rotate tire", qualifier)));

        depot.registerTask(tasks[0]->getName(), tasks[0]->getDescription(), qualifier);
        depot.registerTask(tasks[1]->getName(), tasks[1]->getDescription(), qualifier);
        depot.registerTask(tasks[2]->getName(), tasks[2]->getDescription(), qualifier);
    }

    std::vector<boost::shared_ptr<Task> > tasks;
    boost::shared_ptr<CarQualifierMock> qualifier;
    boost::shared_ptr<CarMock> car;
    TaskDepotImpl depot;
};

bool hasTask(const std::vector<boost::shared_ptr<Task> >& tasks,
    boost::shared_ptr<Task> task)
{
    using namespace boost;
    BOOST_FOREACH(boost::shared_ptr<Task> t, tasks)
    {
        if (t->getName() == task->getName())
        {
            return true;
        }
    }

    return false;
}

TEST_F(TestTaskDepotImpl, listTask)
{
    std::vector<boost::shared_ptr<Task> > ret = depot.listTask();
    EXPECT_EQ(3, tasks.size());
    EXPECT_TRUE(hasTask(ret, tasks[0]));
    EXPECT_TRUE(hasTask(ret, tasks[1]));
    EXPECT_TRUE(hasTask(ret, tasks[2]));
}

TEST_F(TestTaskDepotImpl, findTask)
{
    boost::shared_ptr<Task> task = depot.findTask(tasks[0]->getName());

    EXPECT_EQ(task->getName(), tasks[0]->getName());

    EXPECT_THROW(depot.findTask("no such task"), TaskException);
}

TEST_F(TestTaskDepotImpl, findTaskForCar)
{
    EXPECT_CALL(*qualifier, isQualified(_))
        .Times(3)
        .WillOnce(Return(true))
        .WillRepeatedly(Return(false));

    EXPECT_EQ(1, depot.findTask(car).size());
}

TEST_F(TestTaskDepotImpl, deleteTask)
{
    boost::shared_ptr<Task> task = depot.findTask(tasks[0]->getName());
    depot.deleteTask(task);
    EXPECT_EQ(2, depot.listTask().size());
}
