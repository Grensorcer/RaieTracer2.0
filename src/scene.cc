#include "scene.hh"

namespace environment
{
    std::optional<environment::intersection_record>
    Scene::find_closest_intersection(const Ray &r)
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
                if (!l_ir || (r_ir && r_ir->t < l_ir->t))
                    res = r_ir;
                return res;
            });
    }

    display::Colour Scene::compute_light_input(const Vec3 &intersection_point,
                                               const intersection_record &i_r,
                                               int depth)
    {
        auto reflexion = intersection_point
            - i_r.normal * 2 * (intersection_point * i_r.normal.transpose())[0];
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
                || (light_intersection
                    && structures::norm(light_ray.at(light_intersection->t)
                                        - light_ray.origin())
                        < light_distance))
                continue;
            // Diffusion
            diff += (i_r.normal * light_dir.transpose())[0] * light->intensity()
                / structures::norm(light->center() - intersection_point);

            // Specularity
            auto s_sp = (reflexion * light_dir.transpose())[0];
            spec += pow(s_sp >= 0 ? s_sp : 0, std::get<3>(i_r.comps))
                * light->intensity()
                / structures::norm(light->center() - intersection_point);
        }

        return std::get<0>(i_r.comps)
            * (std::get<1>(i_r.comps) * diff + std::get<2>(i_r.comps) * spec
               + ambiant_light_)
            + std::get<0>(i_r.comps)
            * cast_ray(Ray(intersection_point + reflexion * 0.05, reflexion),
                       depth - 1);
    }

    display::Colour Scene::compute_sky(const Ray &r)
    {
        auto gradient = 0.5 * (r.direction()[2] + 1.);
        return (display::Colour(1., 1., 1.) * (1. - gradient)
                + display::Colour(0.5, 0.7, 1.) * gradient)
            * ambiant_light_;
    }

    display::Colour Scene::cast_ray(const Ray &r, int depth)
    {
        auto colour = display::Colour();
        if (depth >= 0)
        {
            auto oi_r = find_closest_intersection(r);
            if (oi_r)
            {
                auto intersection_point = r.at(oi_r->t);
                colour = compute_light_input(intersection_point, oi_r.value(),
                                             depth);
            }
            else
                colour = compute_sky(r);
        }
        return colour;
    }

} // namespace environment
