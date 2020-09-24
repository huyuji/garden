#pragma once
#ifndef idDDBEC807_45D2_4DFA_9F966B1A63F4E9E8
#define idDDBEC807_45D2_4DFA_9F966B1A63F4E9E8

#include "car.h"

namespace AMT
{

class CarQualifierException : public AMTException
{
public:
    explicit CarQualifierException(const std::string& what) 
        : AMTException(what)
    {
    }
};

class CarQualifier
{
public:
    /*!
     * If the car specified match the qualification expected, nothing happens.
     * Otherwise a CarQualifierException will be raised.
     *
     * @param[in]  car  The car to check qualification for.
     */
    virtual void operator() (Car& car) const = 0;

    /*!
     * The function returns true if the car specified match the qualification expected.
     *
     * @param[in]  car  The car to check qualification for.
     */
    virtual bool isQualified(Car& car) const = 0;

    virtual ~CarQualifier()
    {
    }
};

/*!
 * This class qualifies any car.
 */
class AnyCarQualifier : public CarQualifier
{
public:
    virtual void operator() (Car&) const { }
    
    virtual bool isQualified(Car&) const { return true; }
};

/*!
 * This class requires a car to consume electricity.
 */
class ElectricCarQualifier : public CarQualifier
{
public:
    virtual bool isQualified(Car& car) const;

    virtual void operator() (Car& car) const;
};

/*!
 * This class requires a car to consume fuel.
 */
class FuelCarQualifier : public CarQualifier
{
public:
    virtual bool isQualified(Car& car) const;

    virtual void operator() (Car& car) const;
};

} //end of namespace AMT

#endif // header
