#pragma once
#ifndef id71BC7125_613E_4C70_9E9EAF0EA187D272
#define id71BC7125_613E_4C70_9E9EAF0EA187D272

#include <string>

#include "exception.h"

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
                  const Make& make, 
                  const Model& model, 
                  const Power& power,
                  const unsigned int year, 
                  const unsigned int mileage)
        :   plateNo_(plateNo), 
            make_(make), 
            model_(model),
            power_(power), 
            year_(year), 
            mileage_(mileage)
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

    bool operator == (const CarProperties& p) const
    {
        return p.getPlateNo() == plateNo_
            && p.getMake()    == make_
            && p.getModel()   == model_
            && p.getPower()   == power_
            && p.getYear()    == year_
            && p.getMileage() == mileage_;
    }

    bool equal (const CarProperties& p) const
    {
        return p.getPlateNo() == plateNo_;
    }

    bool consumeFuel() const 
    {
        return power_ == Power::Gas 
            || power_ == Power::Diesel 
            || power_ == Power::Hybrid;
    }

    bool consumeElectricity() const 
    { 
        return power_ == Power::Electric || power_ == Power::Hybrid;
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
    }
};

class Car
{
public:
    /*!
     * Get the unique ID of a car.
     */
    virtual unsigned int getId() = 0;

    virtual const CarProperties& getProperties() = 0;

    virtual std::string toString() = 0;

    virtual ~Car() {};
};

} //end of namespace AMT

#endif // header
