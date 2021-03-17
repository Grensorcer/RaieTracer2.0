#include "object.hh"

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

        double s_discriminant = sqrt(discriminant);
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

        res = std::make_optional<>(intersection_record{});
        res->t = t;
        res->normal = normal(r.at(t));
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

    structures::Vec3 Sphere::reflect(const structures::Vec3 &p,
                                     const structures::Vec3 &n) const
    {
        return txt_->reflect(p, n);
    }

    const components Sphere::get_components(const structures::Vec3 &p) const
    {
        return txt_->get_components(p);
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
        res->normal = normal(r.at(t));
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

    const components Plane::get_components(const structures::Vec3 &p) const
    {
        return txt_->get_components(p);
    }

    std::optional<intersection_record>
    Triangle::intersection(const Ray &r) const
    {
        std::optional<intersection_record> res;
        structures::Vec3 n = normal_ * r.direction() >= 0 ? -normal_ : normal_;
        structures::Vec3 e0 = b_ - a_;
        structures::Vec3 e1 = c_ - a_;
        structures::Vec3 h = r.direction() ^ e1;
        float a = e0 * h;
        if (utils::almost_equal(e0 * h, 0))
            return res;

        float f = 1.0 / a;
        structures::Vec3 s = r.origin() - a_;
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

    const components Triangle::get_components(const structures::Vec3 &p) const
    {
        return txt_->get_components(p);
    }

} // namespace environment
