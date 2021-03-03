#pragma once
#include <math.h>

#include "matrix.hh"

namespace utils
{
    bool almost_equal(double a, double b);
    double random_double();
    double random_double(double min, double max);
    structures::Vec3 random_vec3();
    structures::Vec3 random_vec3(double min, double max);
} // namespace utils
