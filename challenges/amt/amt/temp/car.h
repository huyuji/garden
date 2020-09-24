#pragma once
#ifndef id71BC7125_613E_4C70_9E9EAF0EA187D272
#define id71BC7125_613E_4C70_9E9EAF0EA187D272

#include <string>
#include "boost\thread.hpp"

typedef std::string Make;
typedef std::string Model;

class Car
{
public:
    Car(Make make, Model model, unsigned int year, unsigned int mileage)
        : make_(make), model_(model), year_(year), mileage_(mileage), 
          id_(IdGenerator::next())
    {};
    
    unsigned int getId() const { return id_; };
    
    const Make& getMake() const { return make_; };
    void setMake(const Make& make) { make_ = make; };
    
    const Model& getModel() const { return model_; }
    void setModel(const Model& model) { model_ = model; };
    
    unsigned int getYear() const { return year_; };
    void setYear(const unsigned int year) { year_ = year; };

    unsigned int getMileage() const { return mileage_; }
    void setMileage(const unsigned int mileage) { mileage_ = mileage; };

private:
    class IdGenerator
    {
    public:
        static unsigned int next()
        {
            boost::unique_lock<boost::mutex> lock(mutex_);
            return id_++;
        };

    private:
        static boost::mutex mutex_;
        static unsigned int id_;
    };

    Car(Car&);
    
    unsigned int id_;
    Make make_;
    Model model_;
    unsigned int year_;
    unsigned int mileage_;
};

class ElectricCar : public Car
{
public:
    ElectricCar(Make make, Model model, unsigned int year, unsigned int mileage)
        : Car(make, model, year, mileage)
    {};
};

class DieselCar : public Car
{
public:
    DieselCar(Make make, Model model, unsigned int year, unsigned int mileage)
        : Car(make, model, year, mileage)
    {};
};

class GasCar : public Car
{
public:
    GasCar(Make make, Model model, unsigned int year, unsigned int mileage)
        : Car(make, model, year, mileage)
    {};
};

#endif // header
