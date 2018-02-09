#include "job.h"

using namespace AMT;

IdGenerator Job::idGenerator_;

bool operator == (const boost::shared_ptr<Job>& j1, const boost::shared_ptr<Job>& j2)
{
    return j1->getId() == j2->getId();
}