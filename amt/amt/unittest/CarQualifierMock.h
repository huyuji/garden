#pragma once
#ifndef id9BD25A3A_A9B7_4989_9DB5175B9190DFC2
#define id9BD25A3A_A9B7_4989_9DB5175B9190DFC2

#include <gmock/gmock.h>

#include "..\carQualifier.h"

namespace AMT {

class CarQualifierMock : public AnyCarQualifier 
{
public:
    CarQualifierMock() {}

    MOCK_CONST_METHOD1(functionCall, void(Car&));
    void operator() (Car& car) const { functionCall(car); }

    MOCK_CONST_METHOD1(isQualified, bool(Car&));
};

} //end of namespace AMT

#endif // header
