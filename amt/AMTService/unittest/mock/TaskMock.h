#pragma once
#ifndef idAA8E81F5_C2E5_41DD_A2DC94EB5684377E
#define idAA8E81F5_C2E5_41DD_A2DC94EB5684377E

#include <gmock/gmock.h>

#include "task.h"

namespace AMT {

class TaskMock : public Task
{
public:
    MOCK_METHOD0(getId, unsigned int());

    MOCK_METHOD0(getName, const std::string&());

    MOCK_METHOD0(getDescription, const std::string&());

    MOCK_METHOD0(toString, std::string());

    MOCK_METHOD1(isQualified, bool(Car&));

    MOCK_METHOD1(verify, void(Car& car));
};

}//end of namespace AMT

#endif // header
