#pragma once

#include <algorithm>
#include <memory>
#include <optional>
#include <tuple>

#include "matrix.hh"
#include "ray.hh"
#include "texture.hh"

namespace environment
{
    typedef std::tuple<display::Colour, double, double, double> components;

    struct intersection_record
    {
        double t;
        structures::Vec3 normal;
        structures::Vec3 reflected;
        components comps;
    };

    class Object
    {
    public:
        Object(const structures::Vec3 &center, Texture_Material *txt)
            : txt_{ txt }
            , center_{ center } {};

        virtual ~Object() = default;
        virtual std::optional<intersection_record>
        intersection(const Ray &r) const = 0;
        virtual structures::Vec3 normal(const structures::Vec3 &p) const = 0;
        virtual structures::Vec3 reflect(const structures::Vec3 &p) const = 0;
        virtual const components
        get_components(const structures::Vec3 &p) const = 0;
        virtual const structures::Vec3 at(double i, double j) const = 0;

        const structures::Vec3 &center() const
        {
            return center_;
        }

    protected:
        std::unique_ptr<Texture_Material> txt_;
        structures::Vec3 center_;
    };

    class Sphere : public Object
    {
    public:
        Sphere(const structures::Vec3 &center, Texture_Material *txt,
               const double &radius)
            : Object(center, txt)
            , r_{ radius }
        {}

        const double &radius() const
        {
            return r_;
        }

        const structures::Vec3 at(double i, double j) const override;

        std::optional<intersection_record>
        intersection(const Ray &r) const override;

        structures::Vec3 reflect(const structures::Vec3 &p) const override;
        structures::Vec3 normal(const structures::Vec3 &p) const override;

        const components
        get_components(const structures::Vec3 &p) const override;

    protected:
        double r_;
    };
} // namespace environment
