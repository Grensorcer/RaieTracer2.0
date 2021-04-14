#include "blob.hh"

namespace environment
{
    Blob::Blob(std::shared_ptr<Texture_Material> txt,
               const structures::Vec3 &center, double step, double side,
               double isosurface)
        : txt_{ txt }
        , nmap_{ std::make_shared<Identity_Map>() }
        , center_{ center }
        , step_{ step }
        , side_{ side }
        , isosurface_{ isosurface }
    {
        size_t n = (side / step) + 1;
        auto square = n * n;
        auto cube = n * square;
        potentials_ = std::vector<double>(cube, -1.);
        points_.reserve(cube);
        structures::Vec3 lower_left =
            center - structures::Vec3({ { side / 2., side / 2., side / 2. } });
        auto generator = [&, idx = 0]() mutable {
            auto z = idx / square;
            auto tmp_ = idx - (z * square);
            auto y = tmp_ / n;
            auto x = tmp_ % n;
            ++idx;
            return lower_left
                + structures::Vec3({ { x * step, y * step, z * step } });
        };

        std::generate_n(std::back_inserter(points_), cube, generator);
    }

    Blob::Blob(std::shared_ptr<Texture_Material> txt, std::shared_ptr<Map> nmap,
               const structures::Vec3 &center, double step, double side,
               double isosurface)
        : txt_{ txt }
        , nmap_{ nmap }
        , center_{ center }
        , step_{ step }
        , side_{ side }
        , isosurface_{ isosurface }
    {
        size_t n = (side / step) + 1;
        auto square = n * n;
        auto cube = n * square;
        potentials_ = std::vector<double>(cube, -1.);
        points_.reserve(cube);
        structures::Vec3 lower_left =
            center - structures::Vec3({ { side / 2., side / 2., side / 2. } });
        auto generator = [&, idx = 0]() mutable {
            auto z = idx / square;
            auto tmp_ = idx - (z * square);
            auto y = tmp_ / n;
            auto x = tmp_ % n;
            ++idx;
            return lower_left
                + structures::Vec3({ { x * step, y * step, z * step } });
        };

        std::generate_n(std::back_inserter(points_), cube, generator);
    }

    Blob &Blob::add_energy(const structures::Vec3 &energy)
    {
        energies_.push_back(energy);
        return *this;
    }

    double Blob::growth(const structures::Vec3 &p)
    {
        double potential = 0;
        for (const auto &energy : energies_)
        {
            double t = 1 / structures::norm_squared(energy - p);
            potential += t * t;
        }

        return potential;
    }

    structures::Vec3 Blob::interpolate_vertex(size_t i1, size_t i2)
    {
        return (potentials_[i1] * points_[i1] + potentials_[i2] * points_[i2])
            / (potentials_[i1] + potentials_[i2]);
    }

    void Blob::build_triangles(
        const cube &c, std::array<int, 15> vertices,
        std::vector<std::shared_ptr<Smooth_Triangle>> &triangles)
    {
        for (size_t i = 0; i < 15 && vertices[i] != -1; i += 3)
        {
            auto v1 = interpolate_vertex(c[std::get<1>(sides_[vertices[i]])],
                                         c[std::get<0>(sides_[vertices[i]])]);
            auto v2 =
                interpolate_vertex(c[std::get<1>(sides_[vertices[i + 1]])],
                                   c[std::get<0>(sides_[vertices[i + 1]])]);
            auto v3 =
                interpolate_vertex(c[std::get<1>(sides_[vertices[i + 2]])],
                                   c[std::get<0>(sides_[vertices[i + 2]])]);

            auto triangle =
                std::make_shared<Smooth_Triangle>(txt_, nmap_, v1, v2, v3);
            triangles.emplace_back(triangle);
        }
    }

    std::shared_ptr<Mesh> Blob::marching_cubes()
    {
        std::vector<std::shared_ptr<Smooth_Triangle>> res;
        size_t n = side_ / step_;
        size_t n_cube = n * n * n;
        res.reserve(3 * n_cube);
        for (size_t i = 0; i < n_cube; ++i)
        {
            auto c = get_cube(i);
            int index = 0;
            index |= get_potential(c[0]) > isosurface_ ? 1 : 0;
            index |= get_potential(c[1]) > isosurface_ ? 2 : 0;
            index |= get_potential(c[2]) > isosurface_ ? 4 : 0;
            index |= get_potential(c[3]) > isosurface_ ? 8 : 0;
            index |= get_potential(c[4]) > isosurface_ ? 16 : 0;
            index |= get_potential(c[5]) > isosurface_ ? 32 : 0;
            index |= get_potential(c[6]) > isosurface_ ? 64 : 0;
            index |= get_potential(c[7]) > isosurface_ ? 128 : 0;
            build_triangles(c, triangle_table_[index], res);
        }

        return std::make_shared<Mesh>(res, center_, std::sqrt(3) * side_);
    }

} // namespace environment
