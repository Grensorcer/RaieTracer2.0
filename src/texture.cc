#include "texture.hh"

namespace environment
{
    std::tuple<display::Colour, double, double, double, double>
    Uniform_Texture::get_components(double u, double v) const
    {
        (void)u;
        (void)v;
        return std::make_tuple<>(c_, kd_, ks_, ns_, reflexion_);
    }

    structures::Vec3 Uniform_Smooth::reflect(const structures::Vec3 &p,
                                             const structures::Vec3 &n) const
    {
        (void)p;
        (void)n;
        return structures::Vec3({ { 0, 0, 0 } });
    }

    structures::Vec3 Uniform_Metal::reflect(const structures::Vec3 &p,
                                            const structures::Vec3 &n) const
    {
        return structures::unit(p - n * 2 * p * n);
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

    structures::Vec3 Image_Texture::reflect(const structures::Vec3 &p,
                                            const structures::Vec3 &n) const
    {
        return structures::unit(reflexion_ * (p - n * 2 * p * n));
    }

    std::tuple<display::Colour, double, double, double, double>
    Image_Texture::get_components(double u, double v) const
    {
        size_t i = v * (double)im_.width();
        size_t j = u * (double)im_.height();
        return std::make_tuple<>(im_.get_pixel(j, i), kd_, ks_, ns_,
                                 reflexion_);
    }

} // namespace environment
