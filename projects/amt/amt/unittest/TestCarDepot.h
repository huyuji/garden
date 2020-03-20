#include <gtest\gtest.h>

#include <boost\foreach.hpp>

#include "..\car.h"

using namespace AMT;

class TestCarDepot : public ::testing::Test {
protected:

    PlateNo generatePlate()
    {
        static unsigned int count_ = 1;
        static boost::mutex mutex_;

        boost::unique_lock<boost::mutex> lock(mutex_);
        count_++;
        return "Plate" + boost::lexical_cast<std::string>(count_);
    }

    virtual void SetUp() {
        props.push_back(CarProperties(
            PlateNo(generatePlate()),
            Make("Ford"),
            Model("Mustang"),
            Power::Gas,
            2011,
            10000));
        props.push_back(CarProperties(
            PlateNo(generatePlate()),
            Make("Toyoto"),
            Model("Camry"),
            Power::Gas,
            2011,
            10000));

        depot.registerCar(props[0]);
        depot.registerCar(props[1]);
    }

    std::vector<CarProperties> props;
    CarDepot depot;

};

bool hasProp(const std::vector<boost::shared_ptr<Car> >& cars,
    const CarProperties& prop)
{
    using namespace boost;
    BOOST_FOREACH(boost::shared_ptr<Car> car, cars)
    {
        if (car->getProperties() == prop)
        {
            return true;
        }
    }

    return false;
}

TEST_F(TestCarDepot, listCar)
{
    std::vector<boost::shared_ptr<Car> > cars = depot.listCar();

    EXPECT_EQ(2, cars.size());
    EXPECT_TRUE(hasProp(cars, props[0]));
    EXPECT_TRUE(hasProp(cars, props[1]));
}

TEST_F(TestCarDepot, findCar)
{
    boost::shared_ptr<Car> car = depot.findCar(props[0].getPlateNo());

    EXPECT_EQ(props[0], car->getProperties());

    ASSERT_THROW(depot.findCar(generatePlate()), CarException);
}

TEST_F(TestCarDepot, updateCarKeepPlateNo)
{
    boost::shared_ptr<Car> car = depot.findCar(props[0].getPlateNo());

    props[0].setMileage(props[0].getMileage() + 1000);
    depot.updateCar(car, props[0]);

    car = depot.findCar(props[0].getPlateNo());
    EXPECT_EQ(props[0], car->getProperties());
}

TEST_F(TestCarDepot, updateCarPlateNo)
{
    boost::shared_ptr<Car> car = depot.findCar(props[0].getPlateNo());

    props[0].setPlateNo(generatePlate());
    depot.updateCar(car, props[0]);

    car = depot.findCar(props[0].getPlateNo());
    EXPECT_EQ(props[0], car->getProperties());
}

TEST_F(TestCarDepot, deleteCar)
{
    boost::shared_ptr<Car> car = depot.findCar(props[0].getPlateNo());
    depot.deleteCar(car);

    ASSERT_THROW(depot.findCar(props[0].getPlateNo()), CarException);
}