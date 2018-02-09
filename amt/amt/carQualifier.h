#pragma once
#ifndef idDDBEC807_45D2_4DFA_9F966B1A63F4E9E8
#define idDDBEC807_45D2_4DFA_9F966B1A63F4E9E8

#include "car.h"
#include "exception.h"
#include "message.h"

namespace AMT
{

class CarQualifierException : public AMTException
{
public:
    explicit CarQualifierException(const std::string& what) : AMTException(what)
    {
    };
};

class CarQualifier
{
public:
    virtual void operator() (Car& car) const = 0;

    virtual bool isQualified(Car& car) const = 0;

    virtual ~CarQualifier()
    {
    };
};

class AnyCarQualifier : public CarQualifier
{
public:
    void operator() (Car&) const { };
    bool isQualified(Car&) const { return true; };
};

class ElectricCarQualifier : public CarQualifier
{
public:
    bool isQualified(Car& car) const
    {
        Power power = car.getProperties().getPower(); 
        return power == Power::Electric || power == Power::Hybrid;
    }

    void operator() (Car& car) const
    {
        if (!isQualified(car))
        {
            throw CarQualifierException(NOT_ELECTRICITY_POWERED);
        }
    };
};

class FuelCarQualifier : public CarQualifier
{
public:
    bool isQualified(Car& car) const
    {
        Power power = car.getProperties().getPower();
        return power == Power::Gas || power == Power::Diesel || power == Power::Hybrid;
    }

    void operator() (Car& car) const
    {
        if (isQualified(car))
        {
            throw CarQualifierException(NOT_FUEL_POWERED);
        }
    };
};

} //end of namespace AMT

#endif // header
