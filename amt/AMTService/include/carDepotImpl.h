#pragma once
#ifndef id047006A5_72E1_41D3_9A8873C4DF118130
#define id047006A5_72E1_41D3_9A8873C4DF118130

#include <map>

#include <boost/thread.hpp>

#include "carDepot.h"

namespace AMT
{

class CarDepotImpl : public CarDepot
{
public:
    CarDepotImpl() {}

    virtual boost::shared_ptr<Car> registerCar(const CarProperties& properties_);

    virtual std::vector<boost::shared_ptr<Car> > listCar();

    virtual boost::shared_ptr<Car> findCar(const std::string& plateNo);

    virtual void updateCar(boost::shared_ptr<Car> car, const CarProperties& properties);

    virtual void deleteCar(boost::shared_ptr<Car> car);

private:
    CarDepotImpl(const CarDepotImpl&);
    CarDepotImpl& operator = (const CarDepotImpl&);

    typedef std::map<std::string, boost::shared_ptr<Car> > CarMap;
    typedef CarMap::iterator CarIter;
    typedef CarMap::const_iterator CCarIter;
    CarMap carByPlateNo; /**< car map indexed by Plate No.. */
};

}//end of namespace AMT

#endif // header
