#include "camera.hh"

namespace environment
{
    Ray Camera::create_ray(double i, double j) const
    {
        return Ray(center_,
                   upper_left_corner_ - i * z_ * v_fov_ - j * x_ * h_fov_);
    }
} // namespace environment
