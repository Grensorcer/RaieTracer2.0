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

    class Object;

    struct intersection_record
    {
        double t;
        structures::Vec3 normal;
        components comps;
    };

    class Object
    {
    public:
        Object(const structures::Vec3 &center, Texture_Material *txt)
            : txt_{ txt }
            , center_{ center } {};

        virtual std::optional<intersection_record>
        intersection(const Ray &r) const = 0;
        virtual structures::Vec3 normal(const structures::Vec3 &p) const = 0;
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

        const structures::Vec3 at(double i, double j) const override
        {
            return center()
                + radius()
                * structures::Vec3(
                      { { cos(i) * cos(j), cos(i) * sin(j), sin(j) } });
        }
        std::optional<intersection_record>
        intersection(const Ray &r) const override
        {
            // Translation on ray to center on the sphere
            structures::Vec3 oc = r.origin() - center();
            double a = (r.direction() * r.direction().transpose())[0];
            double b = (2. * r.direction() * oc.transpose())[0];
            double c = (oc * oc.transpose())[0] - radius() * radius();

            double discriminant = b * b - 4 * a * c;
            std::optional<intersection_record> res;
            if (discriminant < 0)
                return res;

            res = std::make_optional<>(intersection_record{});
            if (discriminant == 0)
                res->t = -b / (2 * a);
            else
            {
                double sol1 = (-b - sqrt(discriminant)) / (2 * a);
                double sol2 = (-b + sqrt(discriminant)) / (2 * a);
                structures::Vec3 p1 = r.at(sol1);
                structures::Vec3 p2 = r.at(sol2);
                // Distance between two points
                res->t = sol2 >= 0
                        && structures::norm(oc - p1) > structures::norm(oc - p2)
                    ? sol2
                    : sol1;
            }
            res->normal = normal(r.at(res->t));
            res->comps = get_components(r.at(res->t));

            return res;
        }
        structures::Vec3 normal(const structures::Vec3 &p) const override
        {
            return structures::unit(p - center());
        }

        const components
        get_components(const structures::Vec3 &p) const override
        {
            return txt_->get_components(p);
        }

    protected:
        double r_;
    };
} // namespace environment
