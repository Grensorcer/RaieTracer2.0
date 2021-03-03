#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <limits>
#include <math.h>
#include <memory>
#include <numeric>
#include <optional>
#include <tuple>
#include <vector>

#include "camera.hh"
#include "image.hh"
#include "light.hh"
#include "matrix.hh"
#include "object.hh"
#include "ray.hh"
#include "utils.hh"

using structures::Vec3;
namespace environment
{
    class Scene
    {
    public:
        Scene();
        Scene(Camera &cam, std::vector<std::shared_ptr<Object>> &objects,
              std::vector<std::shared_ptr<Light>> &lights)
            : cam_{ cam }
            , objects_{ objects }
            , lights_{ lights }
        {}
        Scene(Camera &cam, std::vector<std::shared_ptr<Object>> &objects,
              std::vector<std::shared_ptr<Light>> &lights, double ambiant_light)
            : cam_{ cam }
            , objects_{ objects }
            , lights_{ lights }
            , ambiant_light_{ ambiant_light }
        {}

        std::optional<environment::intersection_record>
        find_closest_intersection(const Ray &r);

        display::Colour compute_light_input(const Vec3 &intersection_point,
                                            const intersection_record &i_r,
                                            int depth);

        display::Colour compute_sky(const Ray &r);

        display::Colour cast_ray(const Ray &r, int depth);

    private:
        Camera cam_;
        std::vector<std::shared_ptr<Object>> objects_;
        std::vector<std::shared_ptr<Light>> lights_;
        double ambiant_light_ = 1.;
    };

} // namespace environment
