#include "matrix.hh"

#include "utils.hh"

namespace structures
{
    double operator*(const Vec3 &lhs, const Vec3 &rhs)
    {
        return (lhs * rhs.transpose())[0];
    }

    Vec3 operator^(const Vec3 &lhs, const Vec3 &rhs)
    {
        return Vec3({ { lhs[1] * rhs[2] - lhs[2] * rhs[1],
                        lhs[2] * rhs[0] - lhs[0] * rhs[2],
                        lhs[0] * rhs[1] - lhs[1] * rhs[0] } });
    }

    Vec3 random()
    {
        return Vec3({ { utils::random_double(), utils::random_double(),
                        utils::random_double() } });
    }

    Vec3 random(double min, double max)
    {
        return Vec3(
            { { utils::random_double(min, max), utils::random_double(min, max),
                utils::random_double(min, max) } });
    }

    Vec3 random_unit1()
    {
        while (true)
        {
            auto p = random(-1, 1);
            if (norm_squared(p) >= 1)
                continue;
            return unit(p);
        }
    }

    Vec3 random_unit2()
    {
        auto rand = random(-1, 1);
        return unit(rand);
    }
} // namespace structures
