#pragma once

#include <tuple>

#include "image.hh"
#include "matrix.hh"
namespace environment
{
    class Texture_Material
    {
    public:
        virtual ~Texture_Material() = default;
        virtual structures::Vec3 reflect(const structures::Vec3 &p,
                                         const structures::Vec3 &n) const = 0;
        virtual std::tuple<display::Colour, double, double, double>
        get_components(const structures::Vec3 &p) const = 0;
    };

    class Uniform_Texture : public Texture_Material
    {
    public:
        Uniform_Texture(const Uniform_Texture &txt) = default;
        Uniform_Texture(display::Colour c, double kd, double ks, double ns)
            : c_{ c }
            , kd_{ kd }
            , ks_{ ks }
            , ns_{ ns }
        {}

        std::tuple<display::Colour, double, double, double>
        get_components(const structures::Vec3 &p) const override;

    protected:
        display::Colour c_;
        double kd_;
        double ks_;
        double ns_;
    };

    class Uniform_Smooth : public Uniform_Texture
    {
        using Uniform_Texture::Uniform_Texture;
        structures::Vec3 reflect(const structures::Vec3 &p,
                                 const structures::Vec3 &n) const override;
    };

    class Uniform_Metal : public Uniform_Texture
    {
        using Uniform_Texture::Uniform_Texture;
        structures::Vec3 reflect(const structures::Vec3 &p,
                                 const structures::Vec3 &n) const override;
    };

    class Uniform_Matte : public Uniform_Texture
    {
        using Uniform_Texture::Uniform_Texture;
        structures::Vec3 reflect(const structures::Vec3 &p,
                                 const structures::Vec3 &n) const override;
    };
} // namespace environment
