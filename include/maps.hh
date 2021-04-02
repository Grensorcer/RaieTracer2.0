#pragma once

#include "image.hh"
#include "matrix.hh"

namespace environment
{
    class Map
    {
    public:
        virtual ~Map() = default;
        virtual structures::Vec3 normal(const structures::Vec3 &n,
                                        const structures::Vec3 &t,
                                        const structures::Vec3 &b, double u,
                                        double v) const = 0;
    };

    class Identity_Map : public Map
    {
    public:
        virtual structures::Vec3 normal(const structures::Vec3 &n,
                                        const structures::Vec3 &t,
                                        const structures::Vec3 &b, double u,
                                        double v) const override;
    };

    class Normal_Map : public Map
    {
    public:
        Normal_Map() = delete;
        Normal_Map(const Normal_Map &map) = default;
        Normal_Map(const display::Image &im)
            : im_{ im } {};
        virtual structures::Vec3 normal(const structures::Vec3 &n,
                                        const structures::Vec3 &t,
                                        const structures::Vec3 &b, double u,
                                        double v) const override;
        structures::Vec3 get_tangent_normal(double u, double v) const;

    protected:
        display::Image im_;
    };
} // namespace environment
