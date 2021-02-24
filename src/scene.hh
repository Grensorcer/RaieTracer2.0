#include <iostream>
#include <math.h>
#include <memory>
#include <optional>
#include <tuple>

#include "matrix.hh"

using structures::Vec3;

namespace environment
{
    class Scene
    {};

    class Ray
    {
    public:
        Ray() = default;
        Ray(const Ray &r) = default;
        Ray(const Vec3 &origin, const Vec3 &direction)
            : origin_{ origin }
            , direction_{ direction }
        {}

        Vec3 at(double t) const
        {
            return t * direction_ + origin_;
        }

        const Vec3 &origin() const
        {
            return origin_;
        }
        const Vec3 &direction() const
        {
            return origin_;
        }

    protected:
        Vec3 origin_;
        Vec3 direction_;
    };

    class Texture_Material
    {
    public:
        virtual const std::tuple<double, double, double>
        get_components(size_t i, size_t j) const = 0;
    };

    class Uniform_Texture : public Texture_Material
    {
    public:
        Uniform_Texture() = delete;
        Uniform_Texture(double kd, double ks, double ns)
            : kd_{ kd }
            , ks_{ ks }
            , ns_{ ns }
        {}

        const std::tuple<double, double, double>
        get_components(size_t i, size_t j) const override
        {
            (void)i;
            (void)j;
            return std::make_tuple<>(kd_, ks_, ns_);
        }

    protected:
        double kd_;
        double ks_;
        double ns_;
    };

    class Object
    {
    public:
        virtual std::optional<double> intersection(const Ray &r) const = 0;
        virtual Vec3 normal(size_t i, size_t j) const = 0;
        virtual const std::tuple<double, double, double>
        get_components(size_t i, size_t j) const = 0;

        const Vec3 &center() const
        {
            return center_;
        }

    protected:
        std::unique_ptr<Texture_Material> txt_;
        Vec3 center_;
    };

    class Sphere : public Object
    {
    public:
        const double &radius() const
        {
            return r_;
        }

        std::optional<double> intersection(const Ray &r) const override
        {
            // Translation on ray to center on the sphere
            Vec3 oc = r.origin() - center();
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
                Vec3 p1 = r.at(sol1);
                Vec3 p2 = r.at(sol2);
                // Distance between two points
                res = structures::norm(oc - p1) > structures::norm(oc - p2)
                    ? sol2
                    : sol1;
            }

            return res;
        }
        Vec3 normal(size_t i, size_t j) const override;

        const std::tuple<double, double, double>
        get_components(size_t i, size_t j) const override
        {
            return txt_->get_components(i, j);
        }

    protected:
        double r_;
    };

    class Light
    {};

    class Camera
    {};

} // namespace environment
