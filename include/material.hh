#pragma once

#include <memory>

#include "maps.hh"
#include "texture.hh"

namespace environment
{
    class Object;
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
        virtual std::tuple<display::Colour, double, double, double>
        get_components(double u, double v) const;
        virtual structures::Vec3 normal(const structures::Vec3 &n,
                                        const structures::Vec3 &t,
                                        const structures::Vec3 &b, double u,
                                        double v) const;
        virtual std::pair<double, double> get_height_intersection(
            const structures::Vec3 &p, const structures::Vec3 &n,
            const structures::Vec3 &t, const structures::Vec3 &b,
            const structures::FixedMatrix<1, 2> &uv) const = 0;

        void set_obj(std::shared_ptr<Object> obj)
        {
            obj_ = obj;
        }

    protected:
        std::shared_ptr<Texture> txt_;
        std::shared_ptr<Map> nmap_;
        std::shared_ptr<Object> obj_;
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
        std::pair<double, double> get_height_intersection(
            const structures::Vec3 &p, const structures::Vec3 &n,
            const structures::Vec3 &t, const structures::Vec3 &b,
            const structures::FixedMatrix<1, 2> &uv) const override;
    };

    class Relief_Material : public Material
    {
    public:
        Relief_Material(std::shared_ptr<Texture> txt, std::shared_ptr<Map> nmap,
                        std::shared_ptr<Image_Texture> hm)
            : Material(txt, nmap)
            , hmap_{ hm }
        {}
        std::pair<double, double> get_height_intersection(
            const structures::Vec3 &p, const structures::Vec3 &n,
            const structures::Vec3 &t, const structures::Vec3 &b,
            const structures::FixedMatrix<1, 2> &uv) const override;

    protected:
        std::shared_ptr<Image_Texture> hmap_;

        std::pair<double, double>
        find_intersection(const structures::Vec3 &p,
                          const structures::FixedMatrix<3, 3> &t2w,
                          const structures::FixedMatrix<3, 3> &w2t,
                          const structures::FixedMatrix<1, 2> &uv) const;
        const structures::FixedMatrix<3, 3>
        t2w(const structures::Vec3 &n, const structures::Vec3 &t,
            const structures::Vec3 &b) const;
        const structures::FixedMatrix<3, 3>
        w2t(const structures::Vec3 &n, const structures::Vec3 &t,
            const structures::Vec3 &b) const;
        double b_factor(const structures::Vec3 &a) const;
    };
} // namespace environment
