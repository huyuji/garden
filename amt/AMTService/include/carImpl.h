#pragma once
#ifndef idF058CB28_359E_4A0F_88FEE85C638DEBEF
#define idF058CB28_359E_4A0F_88FEE85C638DEBEF

#include <boost/thread.hpp>

#include "car.h"
#include "idGenerator.h"

namespace AMT
{

class CarImpl : public Car
{
public:
    CarImpl(const CarProperties& properties)
        : id_(idGenerator_.next()), properties_(properties)
    {
    }

    virtual unsigned int getId();

    virtual const CarProperties& getProperties();

    virtual std::string toString();

private:
    friend class CarDepotImpl; /**< CarDepotImpl needs to access the private constructor to support update car properties. */

    CarImpl(const unsigned int id, const CarProperties& properties)
        : id_(id), properties_(properties)
    {
    }

    CarImpl(const CarImpl&);
    CarImpl& operator = (const CarImpl&);

    static IdGenerator idGenerator_; /**< Used to generate unique ID. */

    const unsigned int id_;
    const CarProperties properties_;
};

} //end of namespace AMT

#endif // header
