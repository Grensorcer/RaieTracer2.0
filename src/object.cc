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
        double b = 2. * r.direction() * oc;
        double c = oc * oc - radius() * radius();

        double discriminant = b * b - 4 * a * c;
        std::optional<intersection_record> res;
        if (discriminant < 0)
            return res;

        double sol1 = (-b - sqrt(discriminant)) / (2 * a);
        double sol2 = (-b + sqrt(discriminant)) / (2 * a);
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
        res->reflected = reflect(r.at(t));

        return res;
    }

    structures::Vec3 Sphere::normal(const structures::Vec3 &p) const
    {
        auto normal = p - center();
        structures::unit(normal);
        return normal;
    }

    structures::Vec3 Sphere::reflect(const structures::Vec3 &p) const
    {
        return txt_->reflect(p, normal(p));
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
        res->reflected = reflect(r.at(t));

        return res;
    }

    structures::Vec3 Plane::reflect(const structures::Vec3 &p) const
    {
        return txt_->reflect(p, normal(p));
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
        double dot_prod = n * r.direction();
        std::cout << "dot_prod: " << dot_prod << '\n';
        if (utils::almost_equal(dot_prod, 0))
            return res;
        double d = n * a_;
        double t = (n * r.origin() + d) / dot_prod;
        structures::Vec3 i = r.at(t);

        structures::Vec3 e0 = b_ - a_;
        structures::Vec3 e1 = c_ - b_;
        structures::Vec3 e2 = a_ - c_;
        structures::Vec3 bc0 = i - a_;
        structures::Vec3 bc1 = i - b_;
        structures::Vec3 bc2 = i - c_;
        std::cout << "t: " << t << '\n'
                  << "n: " << n << '\n'
                  << "d: " << d << '\n';
        if (t <= 0 || n * (e0 ^ bc0) <= 0 || n * (e1 ^ bc1) <= 0
            || n * (e2 ^ bc2) <= 0)
            return res;

        res = std::make_optional<>(intersection_record{});
        res->t = t;
        res->normal = n;
        res->comps = get_components(i);
        res->reflected = reflect(i);

        return res;
    }

    structures::Vec3 Triangle::reflect(const structures::Vec3 &p) const
    {
        return txt_->reflect(p, normal(p));
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
