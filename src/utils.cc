#include "utils.hh"

#include <random>

namespace utils
{
    static constexpr double EPSILON = 0.000001;
    bool almost_equal(double a, double b)
    {
        return fabs(a - b) < EPSILON;
    }

    double random_double()
    {
        static thread_local std::mt19937 generator;
        std::uniform_real_distribution<> d(0., 1.);
        return d(generator);
    }

    double random_double(double min, double max)
    {
        static thread_local std::mt19937 generator;
        std::uniform_real_distribution<> d(min, max);
        return d(generator);
    }

    double fmodulo(double a, double b)
    {
        auto r = std::fmod(a, b);
        return r < 0 ? r + b : r;
    }

} // namespace utils
