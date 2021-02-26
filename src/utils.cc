#include "utils.hh"

namespace utils
{
    static constexpr double EPSILON = 0.001;
    bool almost_equal(double a, double b)
    {
        return fabs(a - b) < EPSILON;
    }
} // namespace utils
