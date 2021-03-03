#include "ray.hh"

namespace environment
{
    structures::Vec3 Ray::at(double t) const
    {
        return t * direction_ + origin_;
    }
} // namespace environment
