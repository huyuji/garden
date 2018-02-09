#pragma once
#ifndef id7FDD24B2_3D05_453D_A08ED231B6E14383
#define id7FDD24B2_3D05_453D_A08ED231B6E14383

#include <gmock/gmock.h>

#include "job.h"

namespace AMT {

class JobMock : public Job
{
public:
    MOCK_METHOD0(getId, unsigned int());

    MOCK_METHOD0(getTask, boost::shared_ptr<Task>());

    MOCK_METHOD0(getCar, boost::shared_ptr<Car>());

    MOCK_METHOD0(getCreateTime, time_t());

    MOCK_METHOD0(getState, JobState());

    MOCK_METHOD0(getLogs, std::vector<JobLogEntry>());

    MOCK_METHOD2(addLog, void(const std::string& userName, const std::string& log));

    MOCK_METHOD3(setState, void(JobState state, const std::string& userName, const std::string& log));

    MOCK_METHOD0(toString, std::string());
};

} //end of namespace AMT

#endif // header
