#pragma once

#include "matrix.hh"

namespace environment
{
    class Ray
    {
    public:
        Ray() = default;
        Ray(const Ray &r) = default;
        Ray(const structures::Vec3 &origin, const structures::Vec3 &direction)
            : origin_{ origin }
            , direction_{ direction }
        {}

        structures::Vec3 at(double t) const;

        const structures::Vec3 &origin() const
        {
            return origin_;
        }
        const structures::Vec3 &direction() const
        {
            return direction_;
        }

    protected:
        structures::Vec3 origin_;
        structures::Vec3 direction_;
    };
} // namespace environment
