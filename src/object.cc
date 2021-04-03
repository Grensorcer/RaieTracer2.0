#include "object.hh"

#include <cmath>

namespace environment
{
    /* const structures::Vec3 Sphere::at(double i, double j) const
    {
        return center()
            + radius()
            * structures::Vec3(
                  { { cos(i) * cos(j), cos(i) * sin(j), sin(j) } });
    }
    */

    std::optional<intersection_record> Sphere::intersection(const Ray &r) const
    {
        // Translation on ray to center on the sphere
        structures::Vec3 oc = r.origin() - center();
        double a = r.direction() * r.direction();
        double b = 2. * (r.direction() * oc);
        double c = oc * oc - radius() * radius();

        double discriminant = b * b - 4 * (a * c);
        std::optional<intersection_record> res;
        if (discriminant < 0)
            return res;

        double s_discriminant = std::sqrt(discriminant);
        a *= 2;
        double sol1 = (-b - s_discriminant) / a;
        double sol2 = (-b + s_discriminant) / a;
        double t;
        structures::Vec3 p1 = r.at(sol1);
        structures::Vec3 p2 = r.at(sol2);
        // Distance between two points
        if (sol1 < 0)
            t = sol2;
        else if (sol2 < 0)
            t = sol1;
        else
            t = structures::norm(oc - p1) > structures::norm(oc - p2) ? sol2
                                                                      : sol1;
        if (t < 0)
            return res;
        auto m_n = map_normal(r.at(t));

        res = std::make_optional<>(intersection_record{});
        res->t = t;
        res->normal = m_n;
        res->comps = get_components(r.at(t));
        res->reflected = reflect(r.at(t), res->normal);

        return res;
    }

    structures::Vec3 Sphere::normal(const structures::Vec3 &p) const
    {
        auto normal = p - center();
        structures::unit(normal);
        return normal;
    }

    structures::Vec3 Sphere::tangent(const structures::Vec3 &p) const
    {
        double phi = std::acos(-(p[1] - center_[1]) / r_);
        auto res = structures::Vec3({ -std::sin(phi), 0, std::cos(phi) });
        structures::unit(res);
        return res;
    }

    structures::Vec3 Sphere::reflect(const structures::Vec3 &p,
                                     const structures::Vec3 &n) const
    {
        return txt_->reflect(p, n);
    }

    structures::Vec3 Sphere::map_normal(const structures::Vec3 &p) const
    {
        auto t = tangent(p);
        auto n = normal(p);
        double theta = std::atan2(-(p[2] - center_[2]), p[0] - center_[0]);
        double phi = std::acos(-(p[1] - center_[1]) / r_);
        return map_->normal(n, t, n ^ t, (theta + M_PI) / (2 * M_PI),
                            phi / M_PI);
    }

    const components Sphere::get_components(const structures::Vec3 &p) const
    {
        double theta = std::atan2(-(p[2] - center_[2]), p[0] - center_[0]);
        double phi = std::acos(-(p[1] - center_[1]) / r_);
        return txt_->get_components((theta + M_PI) / (2 * M_PI), phi / M_PI);
    }

    std::optional<intersection_record> Plane::intersection(const Ray &r) const
    {
        std::optional<intersection_record> res;
        double dot_prod = r.direction() * normal_;
        if (utils::almost_equal(dot_prod, 0))
            return res;
        double t = (center_ - r.origin()) * normal_ / dot_prod;
        if (t <= 0)
            return res;
        res = std::make_optional<>(intersection_record{});
        res->t = t;
        res->normal = map_normal(r.at(t));
        res->comps = get_components(r.at(t));
        res->reflected = reflect(r.at(t), res->normal);

        return res;
    }

    structures::Vec3 Plane::reflect(const structures::Vec3 &p,
                                    const structures::Vec3 &n) const
    {
        return txt_->reflect(p, n);
    }

    structures::Vec3 Plane::normal(const structures::Vec3 &p) const
    {
        (void)p;
        return normal_;
    }

    structures::Vec3 Plane::tangent(const structures::Vec3 &p) const
    {
        // TODO
        return structures::Vec3();
    }

    structures::Vec3 Plane::map_normal(const structures::Vec3 &p) const
    {
        // TODO
        return normal(p);
    }

    const components Plane::get_components(const structures::Vec3 &p) const
    {
        return txt_->get_components(0, 0);
    }

    std::optional<intersection_record>
    Triangle::intersection(const Ray &r) const
    {
        std::optional<intersection_record> res;
        structures::Vec3 e0 = vertices_[1] - vertices_[0];
        structures::Vec3 e1 = vertices_[2] - vertices_[0];
        structures::Vec3 h = r.direction() ^ e1;
        float a = e0 * h;
        if (utils::almost_equal(e0 * h, 0))
            return res;

        float f = 1.0 / a;
        structures::Vec3 s = r.origin() - vertices_[0];
        float u = f * (s * h);
        if (u < 0.0 || u > 1.0)
            return res;

        structures::Vec3 q = s ^ e0;
        float v = f * (r.direction() * q);
        if (v < 0.0 || u + v > 1.0)
            return res;

        float t = f * (e1 * q);
        if (t <= 0)
            return res;

        structures::Vec3 i = r.at(t);
        structures::Vec3 n = map_normal(u, v);
        n = n * r.direction() >= 0 ? -n : n;
        res = std::make_optional<>(intersection_record{});
        res->t = t;
        res->normal = n;
        res->comps = get_components(i);
        res->reflected = reflect(i, n);

        return res;
    }

    structures::Vec3 Triangle::reflect(const structures::Vec3 &p,
                                       const structures::Vec3 &n) const
    {
        return txt_->reflect(p, n);
    }
    structures::Vec3 Triangle::normal(const structures::Vec3 &p) const
    {
        (void)p;
        return normal_;
    }

    structures::Vec3 Triangle::tangent(const structures::Vec3 &p) const
    {
        // TODO
        return structures::Vec3();
    }

    structures::Vec3 Triangle::normal(float u, float v) const
    {
        (void)u;
        (void)v;
        return normal_;
    }

    structures::Vec3 Triangle::map_normal(const structures::Vec3 &p) const
    {
        // TODO
        return normal(p);
    }

    structures::Vec3 Triangle::map_normal(float u, float v) const
    {
        // TODO
        return normal(u, v);
    }

    const components Triangle::get_components(const structures::Vec3 &p) const
    {
        return txt_->get_components(0, 0);
    }

    structures::Vec3 Smooth_Triangle::normal(const structures::Vec3 &p) const
    {
        auto e0 = vertices_[1] - vertices_[0];
        auto e1 = vertices_[2] - vertices_[0];
        auto e2 = p - vertices_[0];
        float n0 = e0 * e0;
        float n1 = e0 * e1;
        float n2 = e1 * e1;
        float n3 = e2 * e0;
        float n4 = e2 * e1;
        float d = n0 * n2 - n1 * n1;

        auto v = (n2 * n3 - n1 * n4) / d;
        auto w = (n0 * n4 - n1 * n3) / d;
        auto u = 1. - v - w;

        return u * normals_[0] + v * normals_[1] + (1 - u - v) * normals_[2];
    }

    structures::Vec3
    Smooth_Triangle::map_normal(const structures::Vec3 &p) const
    {
        // TODO
        return normal(p);
    }

    structures::Vec3 Smooth_Triangle::normal(float u, float v) const
    {
        auto w = 1 - u - v;
        return w * normals_[0] + u * normals_[1] + v * normals_[2];
    }

    structures::Vec3 Smooth_Triangle::map_normal(float u, float v) const
    {
        // TODO
        return normal(u, v);
    }

} // namespace environment
