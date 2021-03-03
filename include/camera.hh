#pragma once
#include "matrix.hh"
#include "ray.hh"

namespace environment
{
    class Camera
    {
    public:
        Camera() = default;
        Camera(const structures::Vec3 &center)
            : center_(center)
        {}
        Camera(const structures::Vec3 &center, double focal_length,
               double v_fov, double h_fov)
            : center_{ center }
            , focal_length_{ focal_length }
            , v_fov_{ v_fov }
            , h_fov_{ h_fov }
        {}
        Camera(const structures::Vec3 &center, const structures::Vec3 &z,
               const structures::Vec3 &focus, double focal_length, double v_fov,
               double h_fov)
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

        const structures::Vec3 &x() const
        {
            return x_;
        };

        const structures::Vec3 &y() const
        {
            return y_;
        };

        const structures::Vec3 &z() const
        {
            return z_;
        };

        const structures::Vec3 &center() const
        {
            return center_;
        }

        const structures::Vec3 &upper_left_corner() const
        {
            return upper_left_corner_;
        }

        Ray create_ray(double i, double j) const;

    protected:
        structures::Vec3 center_;
        structures::Vec3 z_ = structures::Vec3({ { 0, 0, 1 } });
        structures::Vec3 y_ = structures::Vec3({ { 0, -1, 0 } });
        structures::Vec3 x_ = z_ ^ y_;
        double focal_length_ = 1.;
        double v_fov_ = 2.;
        double h_fov_ = 2.;
        structures::Vec3 upper_left_corner_ =
            focal_length_ * y_ + x_ * (h_fov_ / 2.) + z_ * (v_fov_ / 2.);
    };
} // namespace environment
