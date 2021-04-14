#pragma once

#include <tuple>

#include "image.hh"
#include "matrix.hh"
namespace environment
{
    class Texture
    {
    public:
        virtual ~Texture() = default;
        virtual structures::Vec3 reflect(const structures::Vec3 &p,
                                         const structures::Vec3 &n) const = 0;
        virtual std::tuple<display::Colour, double, double, double>
        get_components(double u, double v) const = 0;
    };

    class Uniform_Texture : public Texture
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
        get_components(double u, double v) const override;

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

    class Image_Texture : public Texture
    {
    public:
        Image_Texture(const Image_Texture &txt) = default;
        Image_Texture(const display::Image &im, double kd, double ks, double ns,
                      double reflexion)
            : im_{ im }
            , kd_{ kd }
            , ks_{ ks }
            , ns_{ ns }
            , reflexion_{ reflexion }
        {}

        std::tuple<display::Colour, double, double, double>
        get_components(double u, double v) const override;
        structures::Vec3 reflect(const structures::Vec3 &p,
                                 const structures::Vec3 &n) const override;

    private:
        display::Image im_;
        double kd_;
        double ks_;
        double ns_;
        double reflexion_;
    };
} // namespace environment
