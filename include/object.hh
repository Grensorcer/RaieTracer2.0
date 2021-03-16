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
        Object(Texture_Material *txt)
            : txt_{ txt }
        {}

        virtual ~Object() = default;
        virtual std::optional<intersection_record>
        intersection(const Ray &r) const = 0;
        virtual structures::Vec3 normal(const structures::Vec3 &p) const = 0;
        virtual structures::Vec3 reflect(const structures::Vec3 &p) const = 0;
        virtual const components
        get_components(const structures::Vec3 &p) const = 0;
        // virtual const structures::Vec3 at(double i, double j) const = 0;

        /*
        const structures::Vec3 &center() const
        {
            return center_;
        }
        */

    protected:
        std::shared_ptr<Texture_Material> txt_;
    };

    class Sphere : public Object
    {
    public:
        Sphere(const structures::Vec3 &center, Texture_Material *txt,
               const double &radius)
            : Object(txt)
            , r_{ radius }
            , center_{ center }
        {}

        const double &radius() const
        {
            return r_;
        }

        // const structures::Vec3 at(double i, double j) const override;

        std::optional<intersection_record>
        intersection(const Ray &r) const override;

        structures::Vec3 reflect(const structures::Vec3 &p) const override;
        structures::Vec3 normal(const structures::Vec3 &p) const override;
        const structures::Vec3 &center() const
        {
            return center_;
        }

        const components
        get_components(const structures::Vec3 &p) const override;

    protected:
        double r_;
        structures::Vec3 center_;
    };

    class Triangle : public Object
    {
    public:
        Triangle(Texture_Material *txt, const structures::Vec3 &a,
                 const structures::Vec3 &b, const structures::Vec3 &c)
            : Object(txt)
            , a_{ a }
            , b_{ b }
            , c_{ c }
            , normal_{ (b_ - a_) ^ (c_ - a_) }
        {
            structures::unit(normal_);
        }

        std::optional<intersection_record>
        intersection(const Ray &r) const override;

        structures::Vec3 reflect(const structures::Vec3 &p) const override;
        structures::Vec3 normal(const structures::Vec3 &p) const override;

        const components
        get_components(const structures::Vec3 &p) const override;

    protected:
        structures::Vec3 a_;
        structures::Vec3 b_;
        structures::Vec3 c_;
        structures::Vec3 normal_;
    };

    class Plane : public Object
    {
    public:
        Plane(const structures::Vec3 &center, Texture_Material *txt,
              const structures::Vec3 &normal)
            : Object(txt)
            , normal_{ normal }
            , center_{ center }
        {
            structures::unit(normal);
        }

        // const structures::Vec3 at(double i, double j) const override;

        std::optional<intersection_record>
        intersection(const Ray &r) const override;

        structures::Vec3 reflect(const structures::Vec3 &p) const override;
        structures::Vec3 normal(const structures::Vec3 &p) const override;
        const structures::Vec3 &center() const
        {
            return center_;
        }

        const components
        get_components(const structures::Vec3 &p) const override;

    protected:
        structures::Vec3 normal_;
        structures::Vec3 center_;
    };

} // namespace environment
