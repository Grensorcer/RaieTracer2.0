#include "material.hh"

#include "utils.hh"

namespace environment
{
    structures::Vec3 Material::reflect(const structures::Vec3 &p,
                                       const structures::Vec3 &n) const
    {
        return txt_->reflect(p, n);
    }
    std::tuple<display::Colour, double, double, double, double>
    Material::get_components(double u, double v) const
    {
        return txt_->get_components(u, v);
    }
    structures::Vec3 Material::normal(const structures::Vec3 &n,
                                      const structures::Vec3 &t,
                                      const structures::Vec3 &b, double u,
                                      double v) const
    {
        return nmap_->normal(n, t, b, u, v);
    }

    double Classic_Material::get_depth_intersection(
        const structures::FixedMatrix<1, 2> &tex_t_i,
        const structures::FixedMatrix<1, 2> &dir_t_i) const
    {
        (void)tex_t_i;
        (void)dir_t_i;
        return 1;
    }

    double Relief_Material::get_depth_intersection(
        const structures::FixedMatrix<1, 2> &tex_t_i,
        const structures::FixedMatrix<1, 2> &dir_t_i) const
    {
        const int lin_search_steps = 10;
        const int bin_search_steps = 5;
        double step_depth = 1. / (double)lin_search_steps;
        double step = step_depth;
        double depth = 0;
        double best_depth = 1;

        for (int i = 0; i < lin_search_steps - 1; ++i)
        {
            depth += step;
            auto cur_uv = tex_t_i + dir_t_i * depth;
            auto comps = hm_->get_components(utils::fmodulo(cur_uv[0], 1),
                                             utils::fmodulo(cur_uv[1], 1));
            if (best_depth >= 0.996 && 1 - std::get<0>(comps).r() <= depth)
                best_depth = depth;
        }

        depth = best_depth - step;

        for (int i = 0; i < bin_search_steps; ++i)
        {
            step *= 0.5;
            auto cur_uv = tex_t_i + dir_t_i * depth;
            auto comps = hm_->get_components(utils::fmodulo(cur_uv[0], 1),
                                             utils::fmodulo(cur_uv[1], 1));
            if (depth >= 1. - std::get<0>(comps).r())
            {
                best_depth = depth;
                depth -= 2 * step;
            }

            depth += step;
        }
        return best_depth;
    }

} // namespace environment
