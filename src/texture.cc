#include "texture.hh"

namespace environment
{
    std::tuple<display::Colour, double, double, double>
    Uniform_Texture::get_components(const structures::Vec3 &p) const
    {
        (void)p;
        return std::make_tuple<>(c_, kd_, ks_, ns_);
    }

    structures::Vec3 Uniform_Metal::reflect(const structures::Vec3 &p,
                                            const structures::Vec3 &n) const
    {
        return structures::unit(p - n * 2 * p * n);
        /* (void)p;
        auto reflected = n + structures::random_unit1();
        structures::unit(reflected);
        if (reflected == structures::Vec3())
            return n;
        return reflected; */
    }

    structures::Vec3 Uniform_Matte::reflect(const structures::Vec3 &p,
                                            const structures::Vec3 &n) const
    {
        (void)p;
        auto reflected = n + structures::random_unit1();
        structures::unit(reflected);
        if (reflected == structures::Vec3())
            return n;
        return reflected;
    }

} // namespace environment
