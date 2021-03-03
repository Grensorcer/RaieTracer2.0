#include "light.hh"

namespace environment
{
    std::optional<double> Point_Light::intersection(const Ray &r) const
    {
        structures::Vec3 rhs = r.origin() - center();
        double t = -rhs[0] / r.direction()[0];
        return utils::almost_equal(t * rhs[1] + r.direction()[1], 0)
                && utils::almost_equal(t * rhs[2] + r.direction()[2], 0)
            ? std::make_optional<>(t)
            : std::nullopt;
    }

    double Point_Light::intensity() const
    {
        return intensity_;
    }

} // namespace environment
