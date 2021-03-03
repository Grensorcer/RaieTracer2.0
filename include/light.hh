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
        virtual ~Light() = default;

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
        std::optional<double> intersection(const Ray &r) const override;
        double intensity() const override;
    };
} // namespace environment
