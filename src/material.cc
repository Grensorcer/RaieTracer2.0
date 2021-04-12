#include "material.hh"

#include "object.hh"

namespace environment
{
    double modulo(double a, double b)
    {
        double r = std::fmod(a, b);
        return r < 0 ? r + b : r;
    }
    structures::Vec3 Material::reflect(const structures::Vec3 &p,
                                       const structures::Vec3 &n) const
    {
        return txt_->reflect(p, n);
    }
    std::tuple<display::Colour, double, double, double>
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
    std::pair<double, double> Classic_Material::get_height_intersection(
        const structures::Vec3 &p, const structures::Vec3 &n,
        const structures::Vec3 &t, const structures::Vec3 &b,
        const structures::FixedMatrix<1, 2> &uv) const
    {
        (void)b;
        (void)n;
        (void)t;
        return obj_->map_parametrics(p);
    }

    /*
    std::pair<double, double> Relief_Material::find_intersection(
        const structures::Vec3 &p, const structures::FixedMatrix<3, 3> &t2w,
        const structures::FixedMatrix<3, 3> &w2t,
        const structures::FixedMatrix<1, 2> &uv) const
    {
        const int lin_search_steps = 10;
        const int bin_search_steps = 5;
        auto a = p * w2t;
        double b_fac = b_factor(a);
        const double distance = 1 - b_fac;
        double step = distance / (double)lin_search_steps;
        double step_depth = 1. / (double)lin_search_steps;

        double cur_fac = b_fac;
        double best_height = 1;

        for (int i = 0; i < lin_search_steps; ++i)
        {
            cur_fac += step;
            best_height -= step_depth;
            auto p_vec = a * cur_fac;
            // p_vec[2] = 0;
            auto u_v = obj_->map_parametrics(p_vec * t2w);
            auto map_height =
                std::get<0>(hmap_->get_components(u_v.first, u_v.second)).r();
            if (map_height >= best_height)
            {
                best_height += step_depth;
                cur_fac -= step;
                break;
            }
        }

        for (int i = 0; i < bin_search_steps; ++i)
        {
            step_depth *= 0.5;
            step *= 0.5;

            cur_fac += step;
            best_height -= step_depth;

            auto p_vec = a * cur_fac;
            // p_vec[2] = 0;
            auto u_v = obj_->map_parametrics(p_vec * t2w);
            auto map_height =
                std::get<0>(hmap_->get_components(u_v.first, u_v.second)).r();
            if (map_height >= best_height)
            {
                step_depth = step_depth > 0 ? -step_depth : step_depth;
                step = step > 0 ? -step : step;
            }
        }

        auto t_res = a * cur_fac;
        // t_res[2] = 0; // Projecting on tangent plane (?)
        return obj_->map_parametrics(t_res * t2w);
    }
    */

    std::pair<double, double> Relief_Material::find_intersection(
        const structures::Vec3 &p, const structures::FixedMatrix<3, 3> &t2w,
        const structures::FixedMatrix<3, 3> &w2t,
        const structures::FixedMatrix<1, 2> &uv) const
    {
        auto s = structures::unit(structures::unit(p) * w2t);
        auto ds = structures::FixedMatrix<1, 2>(
            { s[0] * 0.05 / s[2], s[1] * 0.05 / s[2] });
        auto dp = uv;

        const int lin_search_steps = 10;
        const int bin_search_steps = 5;
        double step_depth = 1. / (double)lin_search_steps;
        double step = step_depth;
        double depth = 1;
        double best_depth = 1;

        for (int i = 0; i < lin_search_steps - 1; ++i)
        {
            depth -= step;
            auto cur_uv = dp + ds * depth;
            auto comps = hmap_->get_components(modulo(cur_uv[0], 1),
                                               modulo(cur_uv[1], 1));
            if (1 - std::get<0>(comps).r() <= depth)
                best_depth = depth;
        }
        depth = best_depth - step;
        for (int i = 0; i < bin_search_steps; ++i)
        {
            step *= 0.5;
            auto cur_uv = dp + ds * depth;
            auto comps = hmap_->get_components(modulo(cur_uv[0], 1),
                                               modulo(cur_uv[1], 1));
            if (depth >= 1 - std::get<0>(comps).r())
            {
                best_depth = depth;
                depth -= 2 * step;
            }
            depth += step;
        }

        auto new_uv = dp + ds * best_depth;
        return std::make_pair(modulo(new_uv[0], 1), modulo(new_uv[1], 1));
    }

    const structures::FixedMatrix<3, 3>
    Relief_Material::t2w(const structures::Vec3 &n, const structures::Vec3 &t,
                         const structures::Vec3 &b) const
    {
        return structures::FixedMatrix<3, 3>({ { t[0], t[1], t[2] },
                                               { b[0], b[1], b[2] },
                                               { n[0], n[1], n[2] } });
    }
    const structures::FixedMatrix<3, 3>
    Relief_Material::w2t(const structures::Vec3 &n, const structures::Vec3 &t,
                         const structures::Vec3 &b) const
    {
        return structures::FixedMatrix<3, 3>({ { t[0], b[0], n[0] },
                                               { t[1], b[1], n[1] },
                                               { t[2], b[2], n[2] } });
    }

    double Relief_Material::b_factor(const structures::Vec3 &a) const
    {
        return 1 + 1 / a[2];
    }

    std::pair<double, double> Relief_Material::get_height_intersection(
        const structures::Vec3 &p, const structures::Vec3 &n,
        const structures::Vec3 &t, const structures::Vec3 &b,
        const structures::FixedMatrix<1, 2> &uv) const
    {
        auto t2w_matrix = t2w(n, t, b);
        auto w2t_matrix = w2t(n, t, b);
        auto intersection = find_intersection(p, t2w_matrix, w2t_matrix, uv);
        return intersection;
    }

} // namespace environment
