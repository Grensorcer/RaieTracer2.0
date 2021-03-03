#include "utils.hh"

#include <cstdlib>

namespace utils
{
    static constexpr double EPSILON = 0.001;
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

    //    structures::Vec3 random_vec3()
    //    {
    //        return structures::Vec3(
    //            { { random_double(), random_double(), random_double() } });
    //    }

    //    structures::Vec3 random_vec3(double min, double max)
    //    {
    //        return structures::Vec3(
    //            { { random_double(min, max), random_double(min, max),
    //                random_double(min, max) } });
    //    }
} // namespace utils
