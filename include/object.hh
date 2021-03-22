#pragma once

#include <algorithm>
#include <memory>
#include <optional>
#include <tuple>

#include "matrix.hh"
#include "ray.hh"
#include "texture.hh"

namespace environment
{
    typedef std::tuple<display::Colour, double, double, double> components;
    struct intersection_record
    {
        double t;
        structures::Vec3 normal;
        structures::Vec3 reflected;
        components comps;
    };

    class Object
    {
    public:
        Object(std::shared_ptr<Texture_Material> txt)
            : txt_{ txt }
        {}

        virtual ~Object() = default;
        virtual std::optional<intersection_record>
        intersection(const Ray &r) const = 0;
        virtual structures::Vec3 normal(const structures::Vec3 &p) const = 0;
        virtual structures::Vec3 reflect(const structures::Vec3 &p,
                                         const structures::Vec3 &n) const = 0;
        virtual const components
        get_components(const structures::Vec3 &p) const = 0;
        // virtual const structures::Vec3 at(double i, double j) const = 0;

        /*
        const structures::Vec3 &center() const
        {
            return center_;
        }
        */

    protected:
        std::shared_ptr<Texture_Material> txt_;
    };

    class Sphere : public Object
    {
    public:
        Sphere(const structures::Vec3 &center,
               std::shared_ptr<Texture_Material> txt, const double &radius)
            : Object(txt)
            , r_{ radius }
            , center_{ center }
        {}

        const double &radius() const
        {
            return r_;
        }

        // const structures::Vec3 at(double i, double j) const override;

        std::optional<intersection_record>
        intersection(const Ray &r) const override;

        structures::Vec3 reflect(const structures::Vec3 &p,
                                 const structures::Vec3 &n) const override;
        structures::Vec3 normal(const structures::Vec3 &p) const override;
        const structures::Vec3 &center() const
        {
            return center_;
        }

        const components
        get_components(const structures::Vec3 &p) const override;

    protected:
        double r_;
        structures::Vec3 center_;
    };

    class Triangle : public Object
    {
    public:
        Triangle(std::shared_ptr<Texture_Material> txt,
                 const structures::Vec3 &a, const structures::Vec3 &b,
                 const structures::Vec3 &c)
            : Object(txt)
            , vertices_{ a, b, c }
            , normal_{ (b - a) ^ (c - a) }
        {
            structures::unit(normal_);
        }

        std::optional<intersection_record>
        intersection(const Ray &r) const override;

        structures::Vec3 reflect(const structures::Vec3 &p,
                                 const structures::Vec3 &n) const override;

        structures::Vec3 normal(const structures::Vec3 &p) const override;
        virtual structures::Vec3 normal(float u, float v) const;

        const components
        get_components(const structures::Vec3 &p) const override;

        const structures::Vec3 &a() const
        {
            return vertices_[0];
        }
        const structures::Vec3 &b() const
        {
            return vertices_[1];
        }
        const structures::Vec3 &c() const
        {
            return vertices_[2];
        }

        const structures::Vec3 &normal() const
        {
            return normal_;
        }

    protected:
        std::array<structures::Vec3, 3> vertices_;
        structures::Vec3 normal_;
    };

    class Smooth_Triangle : public Triangle
    {
    public:
        Smooth_Triangle(std::shared_ptr<Texture_Material> txt,
                        const structures::Vec3 &a, const structures::Vec3 &b,
                        const structures::Vec3 &c)
            : Triangle(txt, a, b, c)
            , normals_{ normal_, normal_, normal_ }
        {}

        const structures::Vec3 &normal_a() const
        {
            return normals_[0];
        }
        const structures::Vec3 &normal_b() const
        {
            return normals_[1];
        }
        const structures::Vec3 &normal_c() const
        {
            return normals_[2];
        }

        void fix_normals(Smooth_Triangle &other)
        {
            for (short i = 0; i < 3; ++i)
                for (short j = 0; j < 3; ++j)
                    if (vertices_[i] == other.vertices_[j])
                    {
                        normals_[i] += other.normal_;
                        other.normals_[j] += normal_;
                        ++nb_share_;
                        ++(other.nb_share_);
                        break;
                    }
        }

        void fix_normals()
        {
            normals_[0] /= nb_share_;
            normals_[1] /= nb_share_;
            normals_[2] /= nb_share_;
        }

        structures::Vec3 normal(const structures::Vec3 &p) const override;
        structures::Vec3 normal(float u, float v) const override;
        const structures::Vec3 &normal() const
        {
            return normal_;
        }

    protected:
        std::array<structures::Vec3, 3> normals_;
        size_t nb_share_ = 1;
    };

    class Plane : public Object
    {
    public:
        Plane(const structures::Vec3 &center,
              std::shared_ptr<Texture_Material> txt,
              const structures::Vec3 &normal)
            : Object(txt)
            , normal_{ normal }
            , center_{ center }
        {
            structures::unit(normal);
        }

        // const structures::Vec3 at(double i, double j) const override;

        std::optional<intersection_record>
        intersection(const Ray &r) const override;

        structures::Vec3 reflect(const structures::Vec3 &p,
                                 const structures::Vec3 &n) const override;
        structures::Vec3 normal(const structures::Vec3 &p) const override;
        const structures::Vec3 &center() const
        {
            return center_;
        }

        const components
        get_components(const structures::Vec3 &p) const override;

    protected:
        structures::Vec3 normal_;
        structures::Vec3 center_;
    };

} // namespace environment
