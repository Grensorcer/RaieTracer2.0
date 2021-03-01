#pragma once

#include <memory>
#include <optional>
#include <tuple>

#include "matrix.hh"
#include "ray.hh"
#include "texture.hh"

namespace environment
{
    class Object;

    struct intersection_record
    {
        bool intersected = false;
        double t;
        structures::Vec3 normal;
        std::shared_ptr<Object> object;
    };

    class Object
    {
    public:
        Object(const structures::Vec3 &center, Texture_Material *txt)
            : txt_{ txt }
            , center_{ center } {};

        virtual std::optional<double> intersection(const Ray &r) const = 0;
        virtual structures::Vec3 normal(const structures::Vec3 &p) const = 0;
        virtual const std::tuple<display::Colour, double, double, double>
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

        std::optional<double> intersection(const Ray &r) const override
        {
            // Translation on ray to center on the sphere
            structures::Vec3 oc = r.origin() - center();
            double a = (r.direction() * r.direction().transpose())[0];
            double b = (2. * r.direction() * oc.transpose())[0];
            double c = (oc * oc.transpose())[0] - radius() * radius();

            double discriminant = b * b - 4 * a * c;
            std::optional<double> res;
            if (discriminant == 0)
                res = -b / (2 * a);
            else if (discriminant > 0)
            {
                double sol1 = (-b - sqrt(discriminant)) / (2 * a);
                double sol2 = (-b + sqrt(discriminant)) / (2 * a);
                structures::Vec3 p1 = r.at(sol1);
                structures::Vec3 p2 = r.at(sol2);
                // Distance between two points
                res = structures::norm(oc - p1) > structures::norm(oc - p2)
                        && structures::norm(oc - p2) >= 0
                    ? sol2
                    : sol1;
            }

            return res;
        }
        structures::Vec3 normal(const structures::Vec3 &p) const override
        {
            return structures::unit(p - center());
        }

        const std::tuple<display::Colour, double, double, double>
        get_components(const structures::Vec3 &p) const override
        {
            return txt_->get_components(p);
        }

    protected:
        double r_;
    };
} // namespace environment
