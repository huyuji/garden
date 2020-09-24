#pragma once
#ifndef id3F3F76CC_3DFD_44BD_BA52A0476726EDE2
#define id3F3F76CC_3DFD_44BD_BA52A0476726EDE2

#include <vector>

#include <boost/shared_ptr.hpp>

#include "car.h"

namespace AMT
{

class CarDepot
{
public:
    virtual boost::shared_ptr<Car> registerCar(const CarProperties& properties_) = 0;

    virtual std::vector<boost::shared_ptr<Car> > listCar() = 0;

    virtual boost::shared_ptr<Car> findCar(const std::string& plateNo) = 0;

    virtual void updateCar(boost::shared_ptr<Car> car, const CarProperties& properties) = 0;

    virtual void deleteCar(boost::shared_ptr<Car> car) = 0;

    virtual ~CarDepot() {}
};

}//end of namespace AMT

#endif // header
