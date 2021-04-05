#pragma once

#include <algorithm>
#include <cmath>
#include <memory>
#include <optional>
#include <tuple>

#include "maps.hh"
#include "matrix.hh"
#include "ray.hh"
#include "stl_reader.h"
#include "texture.hh"

namespace environment
{
    using components = std::tuple<display::Colour, double, double, double>;
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
        {
            map_ = std::make_shared<Identity_Map>();
        }
        Object(std::shared_ptr<Texture_Material> txt, std::shared_ptr<Map> m)
            : txt_{ txt }
            , map_{ m }
        {}

        virtual ~Object() = default;
        virtual std::optional<intersection_record>
        intersection(const Ray &r) const = 0;

    protected:
        std::shared_ptr<Texture_Material> txt_;
        std::shared_ptr<Map> map_;
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

        Sphere(const structures::Vec3 &center,
               std::shared_ptr<Texture_Material> txt, std::shared_ptr<Map> m,
               const double &radius)
            : Object(txt, m)
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
                                 const structures::Vec3 &n) const;
        std::pair<double, double> parametrics(const structures::Vec3 &p) const;
        structures::Vec3 normal(const structures::Vec3 &p) const;
        structures::Vec3 tangent(double v) const;
        structures::Vec3 map_normal(const structures::Vec3 &n,
                                    const structures::Vec3 &t, double u,
                                    double v) const;

        const components get_components(double u, double v) const;

        const structures::Vec3 &center() const
        {
            return center_;
        }

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
        {
            auto tmp_a = b - a;
            auto tmp_b = c - a;
            normal_ = tmp_a ^ tmp_b;
            structures::unit(normal_);
            auto tmp_da = structures::norm(tmp_a);
            auto tmp_db = structures::norm(tmp_b);
            auto angle = std::acos((tmp_a * tmp_b) / (tmp_da * tmp_db));
            area_ = ((tmp_a * tmp_b) * std::sin(angle)) / 2;
        }

        Triangle(std::shared_ptr<Texture_Material> txt, std::shared_ptr<Map> m,
                 const structures::Vec3 &a, const structures::Vec3 &b,
                 const structures::Vec3 &c)
            : Object(txt, m)
            , vertices_{ a, b, c }
        {
            auto tmp_a = b - a;
            auto tmp_b = c - a;
            normal_ = tmp_a ^ tmp_b;
            structures::unit(normal_);
            auto tmp_da = structures::norm(tmp_a);
            auto tmp_db = structures::norm(tmp_b);
            auto angle = std::acos((tmp_a * tmp_b) / (tmp_da * tmp_db));
            area_ = ((tmp_a * tmp_b) * std::sin(angle)) / 2;
        }

        std::optional<intersection_record>
        intersection(const Ray &r) const override;

        structures::Vec3 reflect(const structures::Vec3 &p,
                                 const structures::Vec3 &n) const;

        virtual structures::Vec3 normal(double u, double v) const;
        virtual structures::Vec3 tangent(double u, double v) const;
        virtual structures::Vec3 map_normal(double u, double v) const;

        const components get_components(double u, double v) const;

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

        const double &area() const
        {
            return area_;
        }

        const structures::Vec3 &normal() const
        {
            return normal_;
        }

    protected:
        std::array<structures::Vec3, 3> vertices_;
        structures::Vec3 normal_;
        double area_;
    };

    class Smooth_Triangle : public Triangle
    {
    public:
        Smooth_Triangle(std::shared_ptr<Texture_Material> txt,
                        const structures::Vec3 &a, const structures::Vec3 &b,
                        const structures::Vec3 &c)
            : Triangle(txt, a, b, c)
            , normals_{ normal_ * area_, normal_ * area_, normal_ * area_ }
        {}
        Smooth_Triangle(std::shared_ptr<Texture_Material> txt,
                        std::shared_ptr<Map> m, const structures::Vec3 &a,
                        const structures::Vec3 &b, const structures::Vec3 &c)
            : Triangle(txt, m, a, b, c)
            , normals_{ normal_ * area_, normal_ * area_, normal_ * area_ }
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
                        /*
                        auto e0 = vertices_[i] - vertices_[(i + 1) % 3];
                        auto e1 = vertices_[i] - vertices_[(i + 2) % 3];
                        auto angle = std::acos(
                            (e0 * e1)
                            / (structures::norm(e0) * structures::norm(e1)));

                        auto e0_o = vertices_[j] - vertices_[(j + 1) % 3];
                        auto e1_o = vertices_[j] - vertices_[(j + 2) % 3];
                        auto angle_o = std::acos((e0_o * e1_o)
                                                 / (structures::norm(e0_o)
                                                    * structures::norm(e1_o)));
                        */
                        normals_[i] += other.normal_ * other.area_;
                        other.normals_[j] += normal_ * area_;
                        break;
                    }
        }

        void fix_normals()
        {
            structures::unit(normals_[0]);
            structures::unit(normals_[1]);
            structures::unit(normals_[2]);
        }

        structures::Vec3 normal(double u, double v) const override;

        const structures::Vec3 &normal() const
        {
            return normal_;
        }

    protected:
        std::array<structures::Vec3, 3> normals_;
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
        Plane(const structures::Vec3 &center,
              std::shared_ptr<Texture_Material> txt, std::shared_ptr<Map> m,
              const structures::Vec3 &normal)
            : Object(txt, m)
            , normal_{ normal }
            , center_{ center }
        {
            structures::unit(normal);
        }

        // const structures::Vec3 at(double i, double j) const override;

        std::optional<intersection_record>
        intersection(const Ray &r) const override;

        structures::Vec3 reflect(const structures::Vec3 &p,
                                 const structures::Vec3 &n) const;
        structures::Vec3 normal(const structures::Vec3 &p) const;
        structures::Vec3 tangent(const structures::Vec3 &p) const;
        structures::Vec3 map_normal(const structures::Vec3 &p) const;
        const structures::Vec3 &center() const
        {
            return center_;
        }

        const components get_components(const structures::Vec3 &p) const;

    protected:
        structures::Vec3 normal_;
        structures::Vec3 center_;
    };

    using mesh = std::vector<std::shared_ptr<Triangle>>;

    class Mesh : public Object
    {
    public:
        Mesh(mesh triangles, structures::Vec3 bb_center, double bb_radius)
            : Object(std::make_shared<Uniform_Smooth>(display::Colour(0, 0, 0),
                                                      0, 0, 0))
            , triangles_{ triangles }
            , bounding_box_{ std::make_shared<Sphere>(bb_center, txt_,
                                                      bb_radius) }
        {}
        Mesh(std::shared_ptr<Texture_Material> txt, const char *pth,
             structures::Vec3 center, double bb_radius);
        Mesh(std::shared_ptr<Texture_Material> txt, std::shared_ptr<Map> nmap,
             const char *pth, structures::Vec3 center, double bb_radius);

        std::optional<intersection_record>
        intersection(const Ray &r) const override;

    protected:
        mesh triangles_;
        std::shared_ptr<Object> bounding_box_;
    };

} // namespace environment
