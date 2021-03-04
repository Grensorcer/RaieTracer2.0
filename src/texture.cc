#include "texture.hh"

namespace environment
{
    structures::Vec3 Uniform_Texture::reflect(const structures::Vec3 &p,
                                              const structures::Vec3 &n) const
    {
        return structures::unit(p - n * 2 * (p * n.transpose())[0]);
        /* (void)p;
        auto reflected = structures::unit(n + structures::random_unit1());
        if (reflected == structures::Vec3())
            return n;
        return reflected; */
    }

    const std::tuple<display::Colour, double, double, double>
    Uniform_Texture::get_components(const structures::Vec3 &p) const
    {
        (void)p;
        return std::make_tuple<>(c_, kd_, ks_, ns_);
    }
} // namespace environment
