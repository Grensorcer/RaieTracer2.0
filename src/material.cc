#include "material.hh"

#include "object.hh"

namespace environment
{
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
    structures::Vec3 Classic_Material::get_height_intersection(
        const structures::Vec3 &p, const structures::Vec3 &n,
        const structures::Vec3 &t, const structures::Vec3 &b) const
    {
        (void)b;
        (void)n;
        (void)t;
        return p;
    }

    structures::Vec3 Relief_Material::find_intersection(
        const structures::Vec3 &p, const structures::FixedMatrix<3, 3> &t2w,
        const structures::FixedMatrix<3, 3> &w2t) const
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
            auto u_v = obj_->map_parametrics((a * cur_fac) * t2w);
            if (std::get<0>(hmap_->get_components(u_v.first, u_v.second)).r()
                >= best_height)
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

            auto u_v = obj_->map_parametrics((a * cur_fac) * t2w);
            if (std::get<0>(hmap_->get_components(u_v.first, u_v.second)).r()
                >= best_height)
            {
                step_depth = -step_depth;
                step = -step;
            }
        }

        auto t_res = a * cur_fac;
        t_res[2] = 0; // Projecting on tangent plane (?)
        return t_res * t2w;
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
        return 1 - 1 / a[2];
    }

    structures::Vec3 Relief_Material::get_height_intersection(
        const structures::Vec3 &p, const structures::Vec3 &n,
        const structures::Vec3 &t, const structures::Vec3 &b) const
    {
        auto t2w_matrix = t2w(n, t, b);
        auto w2t_matrix = w2t(n, t, b);
        auto intersection = find_intersection(p, t2w_matrix, w2t_matrix);
        return intersection;
    }

} // namespace environment
