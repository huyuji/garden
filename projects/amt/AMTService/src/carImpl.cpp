#include <boost/lexical_cast.hpp>

#include "carImpl.h"
#include "message.h"

namespace AMT
{

IdGenerator CarImpl::idGenerator_;

unsigned int CarImpl::getId()
{
    return id_; 
}

const CarProperties& CarImpl::getProperties() 
{
    return properties_; 
}

std::string CarImpl::toString()
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

}//end of namespace AMT