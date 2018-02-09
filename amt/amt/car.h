#pragma once
#ifndef id71BC7125_613E_4C70_9E9EAF0EA187D272
#define id71BC7125_613E_4C70_9E9EAF0EA187D272

#include <string>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

#include "idGenerator.h"
#include "exception.h"
#include "message.h"

namespace AMT
{

enum PowerEnum
{
    Gas = 0,
    Diesel,
    Electric,
    Hybrid,
};

typedef std::string PlateNo;
typedef std::string Make;
typedef std::string Model;
typedef PowerEnum Power;

class CarProperties
{
public:
    CarProperties(const PlateNo& plateNo,
        const Make& make, const Model& model, const Power& power,
        const unsigned int year, const unsigned int mileage)
        : plateNo_(plateNo), make_(make), model_(model),
        power_(power), year_(year), mileage_(mileage)
    {
    }

    PlateNo      getPlateNo()   const { return plateNo_; }
    Make         getMake()      const { return make_; }
    Model        getModel()     const { return model_; }
    Power        getPower()     const { return power_; }
    unsigned int getYear()      const { return year_; }
    unsigned int getMileage()   const { return mileage_; }

    void setPlateNo(const PlateNo& plateNo)     { plateNo_ = plateNo; }
    void setMake(const Make& make)              { make_ = make; }
    void setModel(const Model& model)           { model_ = model; }
    void setPower(const Power& power)           { power_ = power; }
    void setYear(const unsigned int year)       { year_ = year; }
    void setMileage(const unsigned int mileage) { mileage_ = mileage; }

    bool operator == (const CarProperties& p) const;

    bool equal (const CarProperties& p) const
    {
        return p.getPlateNo() == plateNo_;
    }

private:
    PlateNo plateNo_;
    Make make_;
    Model model_;
    Power power_;
    unsigned int year_;
    unsigned int mileage_;
};

class CarException : public AMTException
{
public:
    explicit CarException(const std::string& what) : AMTException(what)
    {
    };
};

class Car
{
public:
    Car(const CarProperties& properties) 
        : id_(idGenerator_.next()), properties_(properties)
    {
    }

    unsigned int getId() const { return id_; }
    const CarProperties& getProperties() const { return properties_; }

    std::string toString() const
    {
        std::string description = boost::lexical_cast<std::string>(properties_.getYear());
        description += " ";
        description += properties_.getMake();
        description += " ";
        description += properties_.getModel();
        description += " ";
        description += PLATE_NO;
        description += " ";
        description += properties_.getPlateNo();

        return description;
    }

    friend class CarDepot;

private:
    Car(const unsigned int id, const CarProperties& properties)
        : id_(id), properties_(properties)
    {
    }

    Car(Car&);
    Car& operator = (Car&);

    static IdGenerator idGenerator_;

    const unsigned int id_;
    const CarProperties properties_;
};

class CarDepot
{
public:
    CarDepot() {}

    boost::shared_ptr<Car> registerCar(const CarProperties& properties_)
    {
        boost::upgrade_lock<boost::shared_mutex> lock(mutex_);

        if (carByPlateNo.find(properties_.getPlateNo()) != carByPlateNo.end())
        {
            throw CarException((boost::format(DUPLICATE_PLATE_NO) % properties_.getPlateNo()).str());
        }

        boost::shared_ptr<Car> car(new Car(properties_));

        boost::upgrade_to_unique_lock<boost::shared_mutex> exclusiveLock(lock);
        carByPlateNo.insert(std::make_pair(properties_.getPlateNo(), car));

        return car;
    }

    std::vector<boost::shared_ptr<Car> > listCar()
    {
        ReadLock lock(mutex_);

        std::vector<boost::shared_ptr<Car> > cars;
        for (CCarIter it = carByPlateNo.begin(); it != carByPlateNo.end(); ++it)
        {
            cars.push_back(it->second);
        }

        return cars;
    }

    boost::shared_ptr<Car> findCar(const std::string& plateNo)
    {
        ReadLock lock(mutex_);

        CCarIter it = carByPlateNo.find(plateNo);
        if (it == carByPlateNo.end())
        {
            throw CarException((boost::format(PLATE_NO_NOT_FOUND) % plateNo).str());
        }

        return it->second;
    }

    void updateCar(boost::shared_ptr<Car> car, const CarProperties& properties)
    {
        const std::string& oldPlateNo = car->getProperties().getPlateNo();
        const std::string& newPlateNo = properties.getPlateNo();

        boost::upgrade_lock<boost::shared_mutex> lock(mutex_);

        if (oldPlateNo != newPlateNo && 
            carByPlateNo.find(newPlateNo) != carByPlateNo.end())
        {
            throw CarException((boost::format(DUPLICATE_PLATE_NO) % newPlateNo).str());
        }

        boost::shared_ptr<Car> newCar(new Car(car->getId(), properties));

        boost::upgrade_to_unique_lock<boost::shared_mutex> exclusiveLock(lock);

        CarIter it = carByPlateNo.find(oldPlateNo);
        if (oldPlateNo == newPlateNo)
        {
            it->second = newCar;
        }
        else
        {
            carByPlateNo.insert(std::make_pair(newPlateNo, newCar));
            carByPlateNo.erase(it);
        }
    }

    void deleteCar(boost::shared_ptr<Car> car)
    {
        WriteLock lock(mutex_);

        carByPlateNo.erase(car->getProperties().getPlateNo());
    }

private:
    CarDepot(const CarDepot&);
    CarDepot& operator = (const CarDepot&);

    typedef std::map<std::string, boost::shared_ptr<Car> > CarMap;
    typedef CarMap::iterator CarIter;
    typedef CarMap::const_iterator CCarIter;
    CarMap carByPlateNo;

    typedef boost::unique_lock<boost::shared_mutex>  WriteLock;
    typedef boost::shared_lock<boost::shared_mutex>  ReadLock;
    boost::shared_mutex mutex_;
};

} //end of namespace AMT

#endif // header
