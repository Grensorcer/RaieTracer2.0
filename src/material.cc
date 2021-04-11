#include "material.hh"

namespace environment
{
    structures::Vec3 Classic_Material::reflect(const structures::Vec3 &p,
                                               const structures::Vec3 &n) const
    {
        return txt_->reflect(p, n);
    }
    std::tuple<display::Colour, double, double, double>
    Classic_Material::get_components(double u, double v) const
    {
        return txt_->get_components(u, v);
    }
    structures::Vec3 Classic_Material::normal(const structures::Vec3 &n,
                                              const structures::Vec3 &t,
                                              const structures::Vec3 &b,
                                              double u, double v) const
    {
        return nmap_->normal(n, t, b, u, v);
    }
} // namespace environment
