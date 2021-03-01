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

        display::Colour cast_ray(const Ray &r)
        {
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
                for (const auto &light : lights_)
                {
                    auto light_dir =
                        structures::unit(light->center() - intersection_point);
                    if ((normal * light_dir.transpose())[0] < 0)
                        continue;
                    // Diffusion
                    diff += (normal * light_dir.transpose())[0]
                        * light->intensity()
                        / structures::norm(light->center()
                                           - intersection_point);

                    // Specularity
                    auto s = intersection_point
                        - normal * 2
                            * (intersection_point * normal.transpose())[0];
                    spec += pow(fabs((s * light_dir.transpose())[0]),
                                std::get<3>(components))
                        * light->intensity();
                }
                return std::get<0>(components)
                    * ((std::get<1>(components) * diff)
                       + std::get<2>(components) * spec + ambiant_light_);
            }
            return display::Colour(0.6, 0.6, 1.) * ambiant_light_;
        }

    private:
        Camera cam_;
        std::vector<std::shared_ptr<Object>> objects_;
        std::vector<std::shared_ptr<Light>> lights_;
        double ambiant_light_;
    };

} // namespace environment
