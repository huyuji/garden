#include "carQualifier.h"
#include "message.h"

namespace AMT
{
bool ElectricCarQualifier::isQualified(Car& car) const
{
    return car.getProperties().consumeElectricity();
}

void ElectricCarQualifier::operator() (Car& car) const
{
    if (!isQualified(car))
    {
        throw CarQualifierException(NOT_ELECTRICITY_POWERED);
    }
};

bool FuelCarQualifier::isQualified(Car& car) const
{
    return car.getProperties().consumeFuel();
}

void FuelCarQualifier::operator() (Car& car) const
{
    if (!isQualified(car))
    {
        throw CarQualifierException(NOT_FUEL_POWERED);
    }
};

}//end of namespace AMT