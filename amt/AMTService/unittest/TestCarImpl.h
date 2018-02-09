#include <gtest/gtest.h>

#include "carImpl.h"

using namespace AMT;

class TestCarImpl : public ::testing::Test {
protected:
    virtual void SetUp() {
        props.push_back(CarProperties(
            PlateNo("a1"),
            Make("Ford"),
            Model("Mustang"),
            Power::Gas,
            2011,
            10000));
        props.push_back(CarProperties(
            PlateNo("a2"),
            Make("Toyoto"),
            Model("Camry"),
            Power::Gas,
            2011,
            10000));
    }

    std::vector<CarProperties> props;
};

TEST_F(TestCarImpl, getId)
{
    EXPECT_NE(CarImpl(props[0]).getId(), CarImpl(props[1]).getId());
}

TEST_F(TestCarImpl, toString)
{
    EXPECT_EQ(std::string("2011 Ford Mustang Plate No. a1"), CarImpl(props[0]).toString());
}

TEST_F(TestCarImpl, getProperties)
{
    CarImpl car(props[0]);
    EXPECT_EQ(props[0], car.getProperties());

    props[0].setMileage(props[0].getMileage() + 100);
    EXPECT_NE(props[0].getMileage(), car.getProperties().getMileage());
}
