#include "utils.hh"

#include <cstdlib>

namespace utils
{
    static constexpr double EPSILON = 0.000001;
    bool almost_equal(double a, double b)
    {
        return fabs(a - b) < EPSILON;
    }

    double random_double()
    {
        return std::rand() / (RAND_MAX + 1.);
    }

    double random_double(double min, double max)
    {
        return min + random_double() * (max - min);
    }

} // namespace utils
