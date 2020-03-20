#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <iostream>
#include <string>

using namespace boost::accumulators;

typedef double extractor(unsigned int);

void testAccumulator()
{
    // Define an accumulator set for calculating the mean and the
    // 2nd moment ...
    accumulator_set<double, stats< tag::min, tag::mean, tag::max > > acc;

    // push in some data ...
    //acc(std::numeric_limits<double>::infinity());
    
    // Display the results ...
    std::cout << "max:    " << max(acc) << std::endl;
    std::cout << "avg:    " << mean(acc) << std::endl;
    std::cout << "min:    " << min(acc) << std::endl;
    std::cout << "count:  " << count(acc) << std::endl;
};