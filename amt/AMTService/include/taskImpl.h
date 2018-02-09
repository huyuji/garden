#pragma once
#ifndef id688C8095_9E5C_47DB_B14654997990B671
#define id688C8095_9E5C_47DB_B14654997990B671

#include <boost/shared_ptr.hpp>

#include "task.h"
#include "idGenerator.h"
#include "carQualifier.h"

namespace AMT
{

class TaskImpl : public Task
{
public:
    TaskImpl(const std::string& name, const std::string& description,
         boost::shared_ptr< CarQualifier> qualifier) 
       : id_(idGenerator_.next()), name_(name),
         description_(description), qualifier_(qualifier)
    {
    }

    virtual unsigned int getId()                { return id_; }

    virtual const std::string& getName()        { return name_; }

    virtual const std::string& getDescription() { return description_; }
    
    virtual std::string toString()              { return getDescription(); }

    virtual bool isQualified(Car& car);

    virtual void verify(Car& car);

private:
    TaskImpl(const TaskImpl&);
    TaskImpl& operator = (const TaskImpl&);

    static IdGenerator idGenerator_;

    const unsigned int id_;
    const std::string name_;
    const std::string description_;
    boost::shared_ptr< CarQualifier> qualifier_;
};

bool operator == (const boost::shared_ptr<TaskImpl>& t1, const boost::shared_ptr<TaskImpl>& t2);

} //end of namespace AMT

#endif // header
