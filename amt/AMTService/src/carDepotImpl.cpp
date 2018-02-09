#include <boost/format.hpp>

#include "carImpl.h"
#include "carDepotImpl.h"
#include "message.h"

namespace AMT
{

boost::shared_ptr<Car> CarDepotImpl::registerCar(const CarProperties& properties_)
{
    if (carByPlateNo.find(properties_.getPlateNo()) != carByPlateNo.end())
    {
        throw CarException((boost::format(DUPLICATE_PLATE_NO) % properties_.getPlateNo()).str());
    }

    //TODO: could use factory here
    boost::shared_ptr<Car> car(new CarImpl(properties_));

    carByPlateNo.insert(std::make_pair(properties_.getPlateNo(), car));

    return car;
}

std::vector<boost::shared_ptr<Car> > CarDepotImpl::listCar()
{
    std::vector<boost::shared_ptr<Car> > cars;
    for (CCarIter it = carByPlateNo.begin(); it != carByPlateNo.end(); ++it)
    {
        cars.push_back(it->second);
    }

    return cars;
}

boost::shared_ptr<Car> CarDepotImpl::findCar(const std::string& plateNo)
{
    CCarIter it = carByPlateNo.find(plateNo);
    if (it == carByPlateNo.end())
    {
        throw CarException((boost::format(PLATE_NO_NOT_FOUND) % plateNo).str());
    }

    return it->second;
}

void CarDepotImpl::updateCar(boost::shared_ptr<Car> car, const CarProperties& properties)
{
    const std::string& oldPlateNo = car->getProperties().getPlateNo();
    const std::string& newPlateNo = properties.getPlateNo();

    //if plate no. changes, verify new plate no. has not been registered.
    if (oldPlateNo != newPlateNo &&
        carByPlateNo.find(newPlateNo) != carByPlateNo.end())
    {
        throw CarException((boost::format(DUPLICATE_PLATE_NO) % newPlateNo).str());
    }

    //keep the ID
    boost::shared_ptr<Car> newCar(new CarImpl(car->getId(), properties));

    CarIter it = carByPlateNo.find(oldPlateNo);
    if (oldPlateNo == newPlateNo)
    {
        it->second = newCar;
    }
    else
    {
        carByPlateNo.erase(it);
        carByPlateNo.insert(std::make_pair(newPlateNo, newCar));
    }
}

void CarDepotImpl::deleteCar(boost::shared_ptr<Car> car)
{
    carByPlateNo.erase(car->getProperties().getPlateNo());
}

}//end of namespace AMT