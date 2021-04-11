#pragma once

#include <memory>

#include "maps.hh"
#include "texture.hh"

namespace environment
{
    class Material
    {
    public:
        Material(std::shared_ptr<Texture> txt)
            : txt_{ txt }
            , nmap_{ std::make_shared<Identity_Map>() }
        {}
        Material(std::shared_ptr<Texture> txt, std::shared_ptr<Map> nmap)
            : txt_{ txt }
            , nmap_{ nmap }
        {}
        virtual ~Material() = default;
        virtual structures::Vec3 reflect(const structures::Vec3 &p,
                                         const structures::Vec3 &n) const = 0;
        virtual std::tuple<display::Colour, double, double, double>
        get_components(double u, double v) const = 0;
        virtual structures::Vec3 normal(const structures::Vec3 &n,
                                        const structures::Vec3 &t,
                                        const structures::Vec3 &b, double u,
                                        double v) const = 0;

    protected:
        std::shared_ptr<Texture> txt_;
        std::shared_ptr<Map> nmap_;
    };

    class Classic_Material : public Material
    {
    public:
        Classic_Material(std::shared_ptr<Texture> txt)
            : Material(txt)
        {}
        Classic_Material(std::shared_ptr<Texture> txt,
                         std::shared_ptr<Map> nmap)
            : Material(txt, nmap)
        {}

        structures::Vec3 reflect(const structures::Vec3 &p,
                                 const structures::Vec3 &n) const override;
        std::tuple<display::Colour, double, double, double>
        get_components(double u, double v) const override;
        structures::Vec3 normal(const structures::Vec3 &n,
                                const structures::Vec3 &t,
                                const structures::Vec3 &b, double u,
                                double v) const override;
    };
} // namespace environment
