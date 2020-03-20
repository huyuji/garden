#include "car.h"

using namespace AMT;

IdGenerator Car::idGenerator_;

bool CarProperties::operator == (const CarProperties& p) const
{
    return p.getPlateNo() == plateNo_
        && p.getMake() == make_
        && p.getModel() == model_
        && p.getPower() == power_
        && p.getYear() == year_
        && p.getMileage() == mileage_;
}

