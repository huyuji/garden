#pragma once
#ifndef id1EBB90BD_2571_4F42_ABF371F437AE58EC
#define id1EBB90BD_2571_4F42_ABF371F437AE58EC

#include <gmock/gmock.h>

#include "..\car.h"

namespace AMT {

class CarMock : public Car 
{
public:
    CarMock() : Car(CarProperties(PlateNo(), Make(), Model(), Power(), 0, 0)) {}

    MOCK_CONST_METHOD0(getId, unsigned int());

    MOCK_CONST_METHOD0(getProperties, const CarProperties&());

    MOCK_CONST_METHOD0(toString, std::string());
};

} //end of namespace AMT

#endif // header
