#pragma once
#include <optional>

#include "matrix.hh"
#include "ray.hh"
#include "utils.hh"

namespace environment
{
    class Light
    {
    public:
        Light() = default;
        Light(const structures::Vec3 &center, double intensity)
            : center_{ center }
            , intensity_{ intensity } {};

        virtual std::optional<double> intersection(const Ray &r) const = 0;
        virtual double intensity() const = 0;
        const structures::Vec3 &center() const
        {
            return center_;
        }

    protected:
        structures::Vec3 center_;
        double intensity_;
    };

    class Point_Light : public Light
    {
        using Light::Light;
        std::optional<double> intersection(const Ray &r) const override
        {
            structures::Vec3 rhs = r.origin() - center();
            double t = -rhs[0] / r.direction()[0];
            return utils::almost_equal(t * rhs[1] + r.direction()[1], 0)
                    && utils::almost_equal(t * rhs[2] + r.direction()[2], 0)
                ? std::make_optional<>(t)
                : std::nullopt;
        }
        double intensity() const override
        {
            return intensity_;
        }
    };
} // namespace environment
