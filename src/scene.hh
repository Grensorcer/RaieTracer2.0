#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <math.h>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "image.hh"
#include "matrix.hh"
#include "utils.hh"

using structures::Vec3;
namespace environment
{
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
            return direction_;
        }

    protected:
        Vec3 origin_;
        Vec3 direction_;
    };

    class Texture_Material
    {
    public:
        virtual ~Texture_Material() = default;
        virtual const std::tuple<display::Colour, double, double, double>
        get_components(const Vec3 &p) const = 0;
    };

    class Uniform_Texture : public Texture_Material
    {
    public:
        Uniform_Texture(const Uniform_Texture &txt) = default;
        Uniform_Texture(display::Colour c, double kd, double ks, double ns)
            : c_{ c }
            , kd_{ kd }
            , ks_{ ks }
            , ns_{ ns }
        {}

        const std::tuple<display::Colour, double, double, double>
        get_components(const Vec3 &p) const override
        {
            (void)p;
            return std::make_tuple<>(c_, kd_, ks_, ns_);
        }

    protected:
        display::Colour c_;
        double kd_;
        double ks_;
        double ns_;
    };

    class Object
    {
    public:
        Object(const Vec3 &center, Texture_Material *txt)
            : txt_{ txt }
            , center_{ center } {};
        /* ~Object()
        {
            delete txt_;
        } */

        virtual std::optional<double> intersection(const Ray &r) const = 0;
        virtual Vec3 normal(const Vec3 &p) const = 0;
        virtual const std::tuple<display::Colour, double, double, double>
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
        Sphere(const Vec3 &center, Texture_Material *txt, const double &radius)
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
            // std::cout << "CHECKING THIS RAY: \n";
            // std::cout << r.origin();
            // std::cout << r.direction();
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
                        && structures::norm(oc - p2) >= 0
                    ? sol2
                    : sol1;
            }

            return res;
        }
        Vec3 normal(const Vec3 &p) const override
        {
            return structures::unit(p - center());
        }

        const std::tuple<display::Colour, double, double, double>
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
            , z_{ structures::unit(z) }
            , y_{ structures::unit(focus) }
            , focal_length_{ focal_length }
            , v_fov_{ v_fov }
            , h_fov_{ h_fov }
        {}
        const double &focal_length() const
        {
            return focal_length_;
        }
        const double &v_fov() const
        {
            return v_fov_;
        }
        const double &h_fov() const
        {
            return h_fov_;
        }

        const Vec3 &x() const
        {
            return x_;
        };

        const Vec3 &y() const
        {
            return y_;
        };

        const Vec3 &z() const
        {
            return z_;
        };

        const Vec3 &center() const
        {
            return center_;
        }

        const Vec3 &upper_left_corner() const
        {
            return upper_left_corner_;
        }

    protected:
        Vec3 center_;
        Vec3 z_ = Vec3({ { 0, 0, 1 } });
        Vec3 y_ = Vec3({ { 0, -1, 0 } });
        Vec3 x_ = z_ ^ y_;
        double focal_length_ = 1.;
        double v_fov_ = 2.;
        double h_fov_ = 2.;
        Vec3 upper_left_corner_ =
            focal_length_ * y_ + x_ * (h_fov_ / 2.) + z_ * (v_fov_ / 2.);
    };

    class Scene
    {
    public:
        Scene();
        Scene(Camera &cam, std::vector<Object *> &objects,
              std::vector<Light *> &lights)
            : cam_{ cam }
            , objects_{ objects }
            , lights_{ lights }
        {}
        Scene(Camera &cam, std::vector<Object *> &objects,
              std::vector<Light *> &lights, double ambiant_light)
            : cam_{ cam }
            , objects_{ objects }
            , lights_{ lights }
            , ambiant_light_{ ambiant_light }
        {}

        display::Colour cast_ray(const Vec3 &origin, const Vec3 &direction)
        {
            auto r = Ray(origin, direction);
            // Gradient
            // auto t = 0.5 * (r.direction()[2] + 1.);
            // auto tmp = 1. - t;
            // return { static_cast<uint8_t>((tmp + t * 0.5) * 255),
            //          static_cast<uint8_t>((tmp + t * 0.7) * 255),
            //          static_cast<uint8_t>((tmp + t * 1.) * 255) };

            // Intersections
            auto imin = 0;
            auto min_distance = std::numeric_limits<double>::max();
            auto intersection_val = 0.;
            for (size_t i = 0; i + objects_.begin() != objects_.end(); ++i)
            {
                auto intersection = objects_[i]->intersection(r);
                if (!intersection.has_value() || intersection.value() < 0)
                    continue;
                auto distance =
                    structures::norm(r.at(intersection.value()) - r.origin());
                if (min_distance > distance)
                {
                    imin = i;
                    min_distance = distance;
                    intersection_val = intersection.value();
                }
            }

            if (!utils::almost_equal(min_distance,
                                     std::numeric_limits<double>::max()))
            {
                // return std::get<0>(
                //    objects_[imin]->get_components(r.at(intersection_val)));
                auto intersection_point = r.at(intersection_val);
                auto components =
                    objects_[imin]->get_components(intersection_point);
                auto normal = objects_[imin]->normal(intersection_point);
                double diff = 0.;
                double spec = 0.;
                for (Light *light : lights_)
                {
                    auto light_dir =
                        structures::unit(light->center() - intersection_point);
                    // Diffusion
                    diff += fabs((normal * light_dir.transpose())[0])
                        * light->intensity()
                        / structures::norm(light->center()
                                           - intersection_point);

                    // Specularity
                    auto s = intersection_point
                        - normal * 2
                            * fabs(
                                (intersection_point * normal.transpose())[0]);
                    spec += pow(fabs((s * light_dir.transpose())[0]),
                                std::get<3>(components))
                        * light->intensity();
                }
                diff += ambiant_light_;
                return std::get<0>(components)
                    * ((std::get<1>(components) * diff)
                       + std::get<2>(components) * spec);
            }
            return display::Colour(255, 255, 255) * ambiant_light_;
        }

    private:
        Camera cam_;
        std::vector<Object *> objects_;
        std::vector<Light *> lights_;
        double ambiant_light_;
    };

} // namespace environment
