#pragma once
#ifndef id5399450E_C078_4258_9CBD21BF19A81909
#define id5399450E_C078_4258_9CBD21BF19A81909

#include "car.h"
#include "exception.h"

namespace AMT
{

class TaskException : public AMTException
{
public:
    explicit TaskException(const std::string& what) : AMTException(what)
    {
    }
};

/*!
 * This class represents a task which can be performed on qualified cars,
 * such change oil or rotate tire.
 */
class Task
{
public:
    /*!
     * Get the unique ID of a task.
     */
    virtual unsigned int getId() = 0;

    virtual const std::string& getName() = 0;

    virtual const std::string& getDescription() = 0;
    
    virtual std::string toString() = 0;

    virtual bool isQualified(Car& car) = 0;

    virtual void verify(Car& car) = 0;

    virtual ~Task() {}
};

} //end of namespace AMT

#endif // header