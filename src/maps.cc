#include "maps.hh"

namespace environment
{
    structures::Vec3 Identity_Map::normal(const structures::Vec3 &n,
                                          const structures::Vec3 &t,
                                          const structures::Vec3 &b, double u,
                                          double v) const
    {
        (void)u;
        (void)v;
        (void)t;
        (void)b;
        return n;
    }

    structures::Vec3 Normal_Map::get_tangent_normal(double u, double v) const
    {
        auto colour = im_.get_pixel(u * im_.height(), v * im_.width());
        return structures::unit(
            structures::Vec3({ colour.r() * 2 - 1, colour.g() * 2 - 1,
                               (colour.b() - 0.5) * 2 }));
    }

    structures::Vec3 Normal_Map::normal(const structures::Vec3 &n,
                                        const structures::Vec3 &t,
                                        const structures::Vec3 &b, double u,
                                        double v) const
    {
        const auto t2w =
            structures::FixedMatrix<3, 3>({ { t[0], t[1], t[2] },
                                            { b[0], b[1], b[2] },
                                            { n[0], n[1], n[2] } });
        auto tangent_normal = get_tangent_normal(u, v);

        return tangent_normal * t2w;
    }
} // namespace environment
