#include "object.hh"

#include <cmath>
#include <numeric>

#include "utils.hh"

namespace environment
{
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
        auto i = r.at(t);
        auto uv = parametrics(i);
        auto map_uv = map_parametrics(i);
        auto sphere_normal = normal(i);
        auto sphere_tangent = tangent(uv.second);
        auto m_n = map_normal(sphere_normal, sphere_tangent, map_uv.first,
                              map_uv.second);

        res = std::make_optional<>(intersection_record{});
        res->t = t;
        res->normal = m_n;
        res->comps = get_components(map_uv.first, map_uv.second);
        res->reflected = reflect(i, res->normal);

        return res;
    }

    std::optional<intersection_record>
    Relief_Sphere::intersection(const Ray &r) const
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
        auto i = r.at(t);
        auto uv = parametrics(i);
        auto map_uv = map_parametrics(i);
        auto sphere_normal = normal(i);
        auto sphere_tangent = tangent(uv.second);
        const double magic_depth_coef = 0.025;
        structures::FixedMatrix<3, 3> w2t_matrix =
            w2t(sphere_normal, sphere_tangent, sphere_normal ^ sphere_tangent);
        auto t_i = i * w2t_matrix;
        auto dir_t_i = structures::FixedMatrix<1, 2>(
            { t_i[0] * magic_depth_coef / t_i[2],
              t_i[1] * magic_depth_coef / t_i[2] });
        auto tex_t_i =
            structures::FixedMatrix<1, 2>({ map_uv.first, map_uv.second });

        auto di = mat_->get_depth_intersection(tex_t_i, dir_t_i);
        auto new_uv = tex_t_i + dir_t_i * di;
        new_uv[0] = utils::fmodulo(new_uv[0], 1);
        new_uv[1] = utils::fmodulo(new_uv[1], 1);

        auto m_n =
            map_normal(sphere_normal, sphere_tangent, new_uv[0], new_uv[1]);

        res = std::make_optional<>(intersection_record{});
        res->t = t;
        res->normal = m_n;
        res->comps = get_components(new_uv[0], new_uv[1]);
        res->reflected = reflect(i, res->normal);

        res->relief = true;
        res->di = di;
        res->light_tex_t_i = new_uv;
        res->w2t = w2t_matrix;
        res->mat = mat_;

        return res;
    }

    structures::FixedMatrix<3, 3>
    Relief_Sphere::w2t(const structures::Vec3 &n, const structures::Vec3 &t,
                       const structures::Vec3 &b) const
    {
        return structures::FixedMatrix<3, 3>({ { t[0], b[0], n[0] },
                                               { t[1], b[1], n[1] },
                                               { t[2], b[2], n[2] } });
    }

    std::pair<double, double>
    Sphere::parametrics(const structures::Vec3 &p) const
    {
        double phi = std::atan2(-(p[1] - center_[1]), p[0] - center_[0]) + M_PI;
        double theta = std::acos(-(p[2] - center_[2]) / r_);
        return std::make_pair<>(theta, phi);
    }

    structures::Vec3 Sphere::normal(const structures::Vec3 &p) const
    {
        auto normal = p - center();
        structures::unit(normal);
        return normal;
    }

    structures::Vec3 Sphere::tangent(double v) const
    {
        auto res = structures::Vec3({ -std::sin(v), 0, std::cos(v) });
        structures::unit(res);
        return res;
    }

    structures::Vec3 Sphere::reflect(const structures::Vec3 &p,
                                     const structures::Vec3 &n) const
    {
        return mat_->reflect(p, n);
    }

    std::pair<double, double>
    Sphere::map_parametrics(const structures::Vec3 &p) const
    {
        auto uv = parametrics(p);
        return std::make_pair(utils::fmodulo(uv.first / M_PI - 0.1, 1),
                              utils::fmodulo(uv.second / (2 * M_PI) + 0.25, 1));
    }

    structures::Vec3 Sphere::map_normal(const structures::Vec3 &n,
                                        const structures::Vec3 &t, double u,
                                        double v) const
    {
        return mat_->normal(n, t, n ^ t, u, v);
    }

    const components Sphere::get_components(double u, double v) const
    {
        return mat_->get_components(u, v);
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
        return mat_->reflect(p, n);
    }

    structures::Vec3 Plane::normal(const structures::Vec3 &p) const
    {
        // TODO
        (void)p;
        return normal_;
    }

    structures::Vec3 Plane::tangent(const structures::Vec3 &p) const
    {
        // TODO
        (void)p;
        return structures::Vec3();
    }

    structures::Vec3 Plane::map_normal(const structures::Vec3 &p) const
    {
        // TODO
        return normal(p);
    }

    const components Plane::get_components(const structures::Vec3 &p) const
    {
        // TODO
        (void)p;
        return mat_->get_components(0, 0);
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
        res->comps = get_components(u, v);
        res->reflected = reflect(i, n);

        return res;
    }

    structures::Vec3 Triangle::reflect(const structures::Vec3 &p,
                                       const structures::Vec3 &n) const
    {
        return mat_->reflect(p, n);
    }

    structures::Vec3 Triangle::tangent(double u, double v) const
    {
        (void)u;
        (void)v;
        return vertices_[1];
    }

    structures::Vec3 Triangle::normal(double u, double v) const
    {
        (void)u;
        (void)v;
        return normal_;
    }

    structures::Vec3 Triangle::map_normal(double u, double v) const
    {
        auto n = normal(u, v);
        auto t = tangent(u, v);
        return mat_->normal(n, t, n ^ t, u, v);
    }

    const components Triangle::get_components(double u, double v) const
    {
        return mat_->get_components(u, v);
    }

    structures::Vec3 Smooth_Triangle::normal(double u, double v) const
    {
        auto w = 1 - u - v;
        return w * normals_[0] + u * normals_[1] + v * normals_[2];
    }

    Mesh::Mesh(std::shared_ptr<Material> mat, const char *pth,
               structures::Vec3 center, double bb_radius)
        : Object(mat)
        , bounding_box_{ std::make_shared<Sphere>(center, mat, bb_radius) }
    {
        stl_reader::StlMesh<double, size_t> m(pth);
        triangles_.reserve(m.num_tris());
        for (size_t i = 0; i < m.num_tris(); ++i)
        {
            const double *c1 = m.tri_corner_coords(i, 0);
            const double *c2 = m.tri_corner_coords(i, 1);
            const double *c3 = m.tri_corner_coords(i, 2);

            auto triangle = std::make_shared<Smooth_Triangle>(
                mat, structures::Vec3({ c1[0], c1[1], c1[2] }) + center,
                structures::Vec3({ c2[0], c2[1], c2[2] }) + center,
                structures::Vec3({ c3[0], c3[1], c3[2] }) + center);

            triangles_.emplace_back(triangle);
        }

        if (triangles_.size() > 0)
        {
            for (size_t i = 0; i < triangles_.size() - 1; ++i)
            {
                std::cout << '\r' << i << " out of " << triangles_.size()
                          << " triangles";
                for (size_t j = i + 1; j < triangles_.size(); ++j)
                    if (triangles_[i]->normal() != triangles_[j]->normal())
                        triangles_[i]->fix_normals(*(triangles_[j]));
                triangles_[i]->fix_normals();
            }
            triangles_[triangles_.size() - 1]->fix_normals();
        }
    }

    std::optional<intersection_record> Mesh::intersection(const Ray &r) const
    {
        std::optional<intersection_record> res;
        if (!bounding_box_->intersection(r))
            return res;

        auto intersection_records =
            std::vector<std::optional<environment::intersection_record>>();

        std::transform(
            triangles_.begin(), triangles_.end(),
            std::back_inserter(intersection_records),
            [r](const auto &object) { return object->intersection(r); });

        res = std::reduce(
            intersection_records.begin(), intersection_records.end(),
            std::optional<environment::intersection_record>(std::nullopt),
            [](auto &l_ir, auto &r_ir) {
                auto res = l_ir;
                if (!l_ir || (r_ir && r_ir->t < l_ir->t))
                    res = r_ir;
                return res;
            });
        if (!res)
            return std::nullopt;

        auto r_from_triangle = Ray(r.at(res->t), res->normal);
        auto i = bounding_box_->intersection(r_from_triangle);
        if (!i)
        {
            std::cerr << "Found no intersection with the bounding box\n";
            return std::nullopt;
        }

        res->comps = i->comps;
        res->normal = i->normal;

        return res;
    }

} // namespace environment
