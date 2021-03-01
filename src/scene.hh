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
        find_closest_intersection(const Ray &r)
        {
            auto intersection_records =
                std::vector<std::optional<environment::intersection_record>>();

            std::transform(
                objects_.begin(), objects_.end(),
                std::back_inserter(intersection_records),
                [r](const auto &object) { return object->intersection(r); });

            return std::reduce(
                intersection_records.begin(), intersection_records.end(),
                std::optional<environment::intersection_record>(std::nullopt),
                [](auto &l_ir, auto &r_ir) {
                    auto res = l_ir;
                    if (!l_ir || l_ir->t < 0
                        || (r_ir.has_value() && r_ir->t >= 0
                            && r_ir->t < l_ir->t))
                        res = r_ir;
                    return res;
                });
        }

        display::Colour compute_light_input(const Ray &r,
                                            const intersection_record &i_r)
        {
            auto intersection_point = r.at(i_r.t);
            double diff = 0.;
            double spec = 0.;
            for (const auto &light : lights_)
            {
                auto light_dir =
                    structures::unit(light->center() - intersection_point);
                auto light_distance =
                    structures::norm(light->center() - intersection_point);
                auto light_ray =
                    Ray(intersection_point + light_dir * 0.05, light_dir);
                auto light_intersection = find_closest_intersection(light_ray);
                if ((i_r.normal * light_dir.transpose())[0] < 0
                    || (light_intersection && light_intersection->t > 0
                        && structures::norm(light_ray.at(light_intersection->t)
                                            - light_ray.origin())
                            < light_distance))
                    continue;
                // Diffusion
                diff += (i_r.normal * light_dir.transpose())[0]
                    * light->intensity()
                    / structures::norm(light->center() - intersection_point);

                // Specularity
                auto s = intersection_point
                    - i_r.normal * 2
                        * (intersection_point * i_r.normal.transpose())[0];
                spec += pow(fabs((s * light_dir.transpose())[0]),
                            std::get<3>(i_r.comps))
                    * light->intensity()
                    / structures::norm(light->center() - intersection_point);
            }

            return std::get<0>(i_r.comps)
                * ((std::get<1>(i_r.comps) * diff)
                   + std::get<2>(i_r.comps) * spec + ambiant_light_);
        }

        display::Colour cast_ray(const Ray &r)
        {
            // Intersections
            auto oi_r = find_closest_intersection(r);
            auto colour = display::Colour();

            if (oi_r && oi_r->t > 0)
                colour = compute_light_input(r, oi_r.value());
            else
                colour = display::Colour(0.6, 0.6, 1.) * ambiant_light_;
            return colour;
        }

    private:
        Camera cam_;
        std::vector<std::shared_ptr<Object>> objects_;
        std::vector<std::shared_ptr<Light>> lights_;
        double ambiant_light_;
    };

} // namespace environment
