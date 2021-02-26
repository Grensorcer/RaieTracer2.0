#include <iostream>
#include <math.h>
#include <memory>
#include <optional>
#include <tuple>

#include "image.hh"
#include "matrix.hh"
#include "utils.hh"

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
        virtual ~Texture_Material();
        virtual const std::tuple<double, double, double>
        get_components(const Vec3 &p) const = 0;
        virtual std::unique_ptr<Texture_Material> make_unique() const = 0;
    };

    class Uniform_Texture : public Texture_Material
    {
    public:
        Uniform_Texture(const Uniform_Texture &txt) = default;
        Uniform_Texture(double kd, double ks, double ns)
            : kd_{ kd }
            , ks_{ ks }
            , ns_{ ns }
        {}
        std::unique_ptr<Texture_Material> make_unique() const override
        {
            return std::make_unique<Uniform_Texture>(*this);
        }

        const std::tuple<double, double, double>
        get_components(const Vec3 &p) const override
        {
            (void)p;
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
        Object(const Vec3 &center, const Texture_Material &txt)
            : txt_{ txt.make_unique() }
            , center_{ center } {};

        virtual std::optional<double> intersection(const Ray &r) const = 0;
        virtual Vec3 normal(const Vec3 &p) const = 0;
        virtual const std::tuple<double, double, double>
        get_components(const Vec3 &p) const = 0;
        virtual const Vec3 at(double i, double j) const = 0;

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
        Sphere(const Vec3 &center, const Texture_Material &txt,
               const double &radius)
            : Object(center, txt)
            , r_{ radius }
        {}

        const double &radius() const
        {
            return r_;
        }

        const Vec3 at(double i, double j) const override
        {
            return center()
                + radius()
                * Vec3({ { cos(i) * cos(j), cos(i) * sin(j), sin(j) } });
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
        Vec3 normal(const Vec3 &p) const override
        {
            return structures::unit(p - center());
        }

        const std::tuple<double, double, double>
        get_components(const Vec3 &p) const override
        {
            return txt_->get_components(p);
        }

    protected:
        double r_;
    };

    class Light
    {
    public:
        Light() = default;
        Light(const Vec3 &center, double intensity)
            : center_{ center }
            , intensity_{ intensity } {};

        virtual std::optional<double> intersection(const Ray &r) const = 0;
        virtual double intensity() const = 0;
        const Vec3 &center() const
        {
            return center_;
        }

    protected:
        Vec3 center_;
        double intensity_;
    };

    class Point_Light : public Light
    {
        using Light::Light;
        std::optional<double> intersection(const Ray &r) const override
        {
            Vec3 rhs = r.origin() - center();
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

    class Camera
    {
    public:
        Camera() = default;
        Camera(const Vec3 &center)
            : center_(center)
        {}
        Camera(const Vec3 &center, double focal_length, double v_fov,
               double h_fov)
            : center_{ center }
            , focal_length_{ focal_length }
            , v_fov_{ v_fov }
            , h_fov_{ h_fov }
        {}
        Camera(const Vec3 &center, const Vec3 &z, const Vec3 &focus,
               double focal_length, double v_fov, double h_fov)
            : center_{ center }
            , z_{ z }
            , y_{ focus }
            , focal_length_{ focal_length }
            , v_fov_{ v_fov }
            , h_fov_{ h_fov }
        {}

        display::Colour cast_ray(double i, double j)
        {
            Vec3 upper_left_corner =
                focal_length_ * y_ + x_ * (h_fov_ / 2.) + z_ * (v_fov_ / 2.);
            auto r = Ray(center_,
                         structures::unit(upper_left_corner + i * z_ + j * x_));
            return {};
        }

    protected:
        Vec3 center_;
        Vec3 z_ = Vec3({ { 0, 0, 1 } });
        Vec3 y_ = Vec3({ { 0, -1, 0 } });
        Vec3 x_ = z_ ^ y_;
        double focal_length_ = 1.;
        double v_fov_ = 2.;
        double h_fov_ = 2.;
    };

} // namespace environment
