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
                                         const structures::Vec3 &n) const;
        virtual std::tuple<display::Colour, double, double, double, double>
        get_components(double u, double v) const;
        virtual structures::Vec3 normal(const structures::Vec3 &n,
                                        const structures::Vec3 &t,
                                        const structures::Vec3 &b, double u,
                                        double v) const;

        virtual double get_depth_intersection(
            const structures::FixedMatrix<1, 2> &dp,
            const structures::FixedMatrix<1, 2> &ds) const = 0;

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

        double get_depth_intersection(
            const structures::FixedMatrix<1, 2> &dp,
            const structures::FixedMatrix<1, 2> &ds) const override;
    };

    class Relief_Material : public Material
    {
    public:
        Relief_Material(std::shared_ptr<Texture> txt, std::shared_ptr<Map> nmap,
                        std::shared_ptr<Image_Texture> hm)
            : Material(txt, nmap)
            , hm_{ hm }
        {}

        double get_depth_intersection(
            const structures::FixedMatrix<1, 2> &dp,
            const structures::FixedMatrix<1, 2> &ds) const override;

    protected:
        std::shared_ptr<Image_Texture> hm_;
    };
} // namespace environment
