#include "scene.hh"

namespace environment
{
    std::optional<environment::intersection_record>
    Scene::find_closest_intersection(const Ray &r) const
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
                                               int depth) const
    {
        double diff = 0.;
        double spec = 0.;
        for (const auto &light : lights_)
        {
            auto intersection_to_light = light->center() - intersection_point;
            auto light_distance = structures::norm(intersection_to_light);
            auto &light_dir = structures::unit(intersection_to_light);
            auto light_ray =
                Ray(intersection_point + light_dir * 0.05, light_dir);
            auto light_intersection = find_closest_intersection(light_ray);
            if (light_intersection
                && structures::norm(light_ray.at(light_intersection->t)
                                    - light_ray.origin())
                    < light_distance)
                continue;
            // Diffusion
            auto diff_angle = i_r.normal * light_dir;
            if (diff_angle > 0)
                diff += diff_angle * light->intensity() / light_distance;

            // Specularity
            auto s_sp = i_r.reflected * light_dir;
            if (s_sp > 0)
            {
                // std::cout << "SPECU: " << s_sp << '\n';
                // std::cout << "POW: " << std::get<3>(i_r.comps) << '\n';
                // std::cout << "GIVES: " << pow(s_sp, std::get<3>(i_r.comps))
                // << '\n';
                spec += pow(s_sp, std::get<3>(i_r.comps)) * light->intensity()
                    / light_distance;
            }
        }

        auto res = std::get<0>(i_r.comps)
                * (std::get<1>(i_r.comps) * diff + ambiant_light_)
            + display::Colour(1., 1., 1.) * std::get<2>(i_r.comps) * spec;
        if (i_r.reflected * i_r.normal > 0)
            res += std::get<0>(i_r.comps)
                * cast_ray(Ray(intersection_point + i_r.reflected * 0.05,
                               i_r.reflected),
                           depth - 1);
        return res;
    }

    display::Colour Scene::compute_sky(const Ray &r) const
    {
        auto gradient = 0.5 * (r.direction()[2] + 1.);
        return (display::Colour(1., 1., 1.) * (1. - gradient)
                + display::Colour(0.5, 0.7, 1.) * gradient)
            * ambiant_light_;
    }

    display::Colour Scene::cast_ray(const Ray &r, int depth) const
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
