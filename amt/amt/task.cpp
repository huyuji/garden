#include "task.h"

using namespace AMT;

IdGenerator Task::idGenerator_;

bool operator == (const boost::shared_ptr<Task>& t1, const boost::shared_ptr<Task>& t2)
{
    return t1->getId() == t2->getId();
}