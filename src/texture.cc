#include "texture.hh"

namespace environment
{
    const std::tuple<display::Colour, double, double, double>
    Uniform_Texture::get_components(const structures::Vec3 &p) const
    {
        (void)p;
        return std::make_tuple<>(c_, kd_, ks_, ns_);
    }
} // namespace environment
