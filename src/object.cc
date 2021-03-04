#include "object.hh"

namespace environment
{
    const structures::Vec3 Sphere::at(double i, double j) const
    {
        return center()
            + radius()
            * structures::Vec3(
                  { { cos(i) * cos(j), cos(i) * sin(j), sin(j) } });
    }

    std::optional<intersection_record> Sphere::intersection(const Ray &r) const
    {
        // Translation on ray to center on the sphere
        structures::Vec3 oc = r.origin() - center();
        double a = (r.direction() * r.direction().transpose())[0];
        double b = (2. * r.direction() * oc.transpose())[0];
        double c = (oc * oc.transpose())[0] - radius() * radius();

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
        return structures::unit(p - center());
    }

    structures::Vec3 Sphere::reflect(const structures::Vec3 &p) const
    {
        return txt_->reflect(p, normal(p));
    }

    const components Sphere::get_components(const structures::Vec3 &p) const
    {
        return txt_->get_components(p);
    }

} // namespace environment
