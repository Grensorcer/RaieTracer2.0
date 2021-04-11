#include "scene_collection.hh"

#include "object.hh"
#include "stl_reader.h"

namespace environment
{
    Scene &scene_one(Scene &s)
    {
        s.add_light(std::make_shared<Point_Light>(
            structures::Vec3({ { -3, -3, 4 } }), 3.));

        s.add_object(std::make_shared<Sphere>(
            structures::Vec3({ { -2, -3, -1 } }),
            std::make_shared<Classic_Material>(std::make_shared<Uniform_Metal>(
                display::Colour(0.25, 0.25, 0.25), 0.8, 1, 1)),
            0.5));
        s.add_object(std::make_shared<Sphere>(
            structures::Vec3({ { 0, -3, -1 } }),
            std::make_shared<Classic_Material>(std::make_shared<Uniform_Matte>(
                display::Colour(0., 0., 1.), 0.8, 0.5, 2)),
            1.));
        s.add_object(std::make_shared<Sphere>(
            structures::Vec3({ { 2, -3, -1 } }),
            std::make_shared<Classic_Material>(std::make_shared<Uniform_Smooth>(
                display::Colour(1., 1., 0.), 0.5, 1., 2.)),
            2.));
        /*
        s.add_object(std::make_shared<Sphere>(
            structures::Vec3({ { 0, -1, -101.5 } }),
            new Uniform_Matte(display::Colour(0, 0.33,
        0.1), 1., 1., 1.), 100.));
        */
        s.add_object(std::make_shared<Plane>(
            structures::Vec3({ { 0, 0, -1 } }),
            std::make_shared<Classic_Material>(std::make_shared<Uniform_Matte>(
                display::Colour(0, 0.33, 0.1), 1., 1., 1.)),
            structures::Vec3({ { 0, 0, 1 } })));

        s.add_object(std::make_shared<Triangle>(
            std::make_shared<Classic_Material>(std::make_shared<Uniform_Metal>(
                display::Colour(1., 1., 1.), 0.01, 0.01, 0.01)),
            structures::Vec3({ { -2.5, -4.5, 1 } }),
            structures::Vec3({ { -2, -5, -1 } }),
            structures::Vec3({ { -4, -4, -1 } })));

        return s;
    }

    Scene &scene_triangles(Scene &s)
    {
        size_t nb = 6;
        auto txt = std::make_shared<Image_Texture>(
            "../data/Paper_Wrinkled_001_basecolor.jpg", 1., 1., 1.);
        auto n_m = std::make_shared<Normal_Map>(
            "../data/Paper_Wrinkled_001_normal.jpg");
        auto mat = std::make_shared<Classic_Material>(txt, n_m);
        for (size_t k = 0; k < nb; ++k)
            for (size_t i = 0; i < nb; ++i)
                for (size_t j = 0; j < nb; ++j)
                    s.add_object(std::make_shared<Triangle>(
                        mat,
                        structures::Vec3({ { 2. - i, -(4. + k), 2. - j } }),
                        structures::Vec3({ { 2. - i, -(4. + k), 3. - j } }),
                        structures::Vec3({ { 3. - i, -(4. + k), 2. - j } })));

        s.add_light(std::make_shared<Point_Light>(
            structures::Vec3({ { 0, -1, 2 } }), 2.));
        s.add_light(std::make_shared<Point_Light>(
            structures::Vec3({ { 0, -5, 5 } }), 5.));

        return s;
    }

    Scene &scene_smooth_triangles(Scene &s)
    {
        auto txt = std::make_shared<Uniform_Smooth>(
            display::Colour(0.5, 0.1, 0.5), 1., 1., 1.);
        auto n_m = std::make_shared<Normal_Map>(
            "../data/Paper_Wrinkled_001_normal.jpg");
        auto mat = std::make_shared<Classic_Material>(txt, n_m);
        size_t nb = 6;
        for (size_t k = 0; k < nb; ++k)
            for (size_t i = 0; i < nb; ++i)
                for (size_t j = 0; j < nb; ++j)
                    s.add_object(std::make_shared<Smooth_Triangle>(
                        mat,
                        structures::Vec3({ { 2. - i, -(4. + k), 2. - j } }),
                        structures::Vec3({ { 2. - i, -(4. + k), 3. - j } }),
                        structures::Vec3({ { 3. - i, -(4. + k), 2. - j } })));

        s.add_light(std::make_shared<Point_Light>(
            structures::Vec3({ { 0, -1, 2 } }), 2.));
        s.add_light(std::make_shared<Point_Light>(
            structures::Vec3({ { 0, -5, 5 } }), 5.));

        return s;
    }

    Scene &scene_triangle_reflect(Scene &s)
    {
        s.add_object(std::make_shared<Sphere>(
            structures::Vec3(),
            std::make_shared<Classic_Material>(std::make_shared<Uniform_Smooth>(
                display::Colour(0.2, 0.3, 0.7), 1., 0.5, 0.5)),
            1.));
        s.add_object(std::make_shared<Triangle>(
            std::make_shared<Classic_Material>(std::make_shared<Uniform_Metal>(
                display::Colour(1., 1., 1.), 0.5, 0.1, 0.1)),
            structures::Vec3({ { -1, -2, -0.5 } }),
            structures::Vec3({ { 1, -2, -0.5 } }),
            structures::Vec3({ { 0, -2, 1.5 } })));
        s.add_object(std::make_shared<Triangle>(
            std::make_shared<Classic_Material>(std::make_shared<Uniform_Metal>(
                display::Colour(0.5, 1., 1.), 0.5, 0.1, 0.1)),
            structures::Vec3({ { -2, 0.5, 3.5 } }),
            structures::Vec3({ { 2, 0.5, 3.5 } }),
            structures::Vec3({ { 0, 2, -1.5 } })));

        s.add_light(std::make_shared<Point_Light>(
            structures::Vec3({ { 2, 0, 2 } }), 2.));

        return s;
    }

    Scene &scene_sphere_reflect(Scene &s)
    {
        s.add_object(std::make_shared<Sphere>(
            structures::Vec3({ { 1.5, -2, 0 } }),
            std::make_shared<Classic_Material>(std::make_shared<Uniform_Metal>(
                display::Colour(0.2, 0.3, 0.7), 1., 0.5, 0.5)),
            1.));
        s.add_object(std::make_shared<Sphere>(
            structures::Vec3({ { -1.5, -2, 0 } }),
            std::make_shared<Classic_Material>(std::make_shared<Uniform_Metal>(
                display::Colour(0.9, 0.9, 0.8), 1., 0.5, 0.5)),
            1.));
        s.add_light(std::make_shared<Point_Light>(
            structures::Vec3({ { 0, -1, 3 } }), 1.));

        return s;
    }

    Scene &scene_sphere_triangle_reflect(Scene &s)
    {
        s.add_object(std::make_shared<Sphere>(
            structures::Vec3({ { 1.5, -2, 0 } }),
            std::make_shared<Classic_Material>(std::make_shared<Uniform_Metal>(
                display::Colour(0.2, 0.3, 0.7), 1., 0.5, 0.5)),
            1.));
        s.add_object(std::make_shared<Triangle>(
            std::make_shared<Classic_Material>(std::make_shared<Uniform_Metal>(
                display::Colour(0.9, 0.9, 0.8), 0.7, 0.1, 0.1)),
            structures::Vec3({ { -1, 2, -2 } }),
            structures::Vec3({ { -1, -6, -2 } }),
            structures::Vec3({ { -1, -2, 6 } })));
        s.add_light(std::make_shared<Point_Light>(
            structures::Vec3({ { 0, -1, 3 } }), 1.));

        return s;
    }

    Scene &scene_blob(Scene &s, const maps &m)
    {
        auto txt = std::make_shared<Image_Texture>(std::get<0>(m), 1., 1., 1.);
        auto n_m = std::make_shared<Normal_Map>(std::get<1>(m));
        auto b = Blob(std::make_shared<Classic_Material>(txt, n_m),
                      structures::Vec3({ { 0, -4, 0 } }), 0.15, 2, 8);
        b.add_energy(structures::Vec3({ { 0.35, -4, -0.35 } }));
        b.add_energy(structures::Vec3({ { -0.35, -4, 0.35 } }));
        auto blob_mesh = b.marching_cubes();

        s.add_object(blob_mesh);

        /*
        s.add_object(std::make_shared<Sphere>(
            structures::Vec3({ { 2, 2, 2 } }),
            std::make_shared<Uniform_Metal>(display::Colour(0.5, 0.5, 0.5), 1.,
                                            0.5, 0.5),
            1.));
            */

        /*
        s.add_object(std::make_shared<Plane>(
            structures::Vec3({ { 0, 0, -2 } }),
            std::make_shared<Uniform_Smooth>(display::Colour(0, 0.33, 0.1), 1.,
                                             1., 1.),
            structures::Vec3({ { 0, 0, 1 } })));
            */

        s.add_light(std::make_shared<Point_Light>(
            structures::Vec3({ { 2, 0, 3 } }), 2));

        return s;
    }

    Scene &scene_mesh(Scene &s, const maps &m, const model &mod)
    {
        auto txt = std::make_shared<Image_Texture>(std::get<0>(m), 1., 1., 1.);
        auto n_m = std::make_shared<Normal_Map>(std::get<1>(m));
        auto mat = std::make_shared<Classic_Material>(txt, n_m);
        auto cup = std::make_shared<Mesh>(
            mat, mod.first, structures::Vec3({ 0, -3, 0 }), mod.second);

        s.add_object(cup);

        /*
        s.add_object(std::make_shared<Plane>(
            structures::Vec3({ { 0, 0, -2 } }),
            std::make_shared<Uniform_Smooth>(display::Colour(0, 0.33, 0.1), 1.,
                                             1., 1.),
            structures::Vec3({ { 0, 0, 1 } })));
        */

        s.add_light(std::make_shared<Point_Light>(
            structures::Vec3({ { -1, -0.5, 2.5 } }), 1));

        return s;
    }

    Scene &scene_sphere_texture_reflect(Scene &s, const maps &m)
    {
        s.add_object(std::make_shared<Sphere>(
            structures::Vec3({ 1.5, -2, 0 }),
            std::make_shared<Classic_Material>(
                std::make_shared<Image_Texture>(std::get<0>(m), 1., 0.7, 0.7),
                std::make_shared<Normal_Map>(std::get<1>(m))),
            1.));
        s.add_object(std::make_shared<Sphere>(
            structures::Vec3({ 0.5, -2, 2.4 }),
            std::make_shared<Classic_Material>(std::make_shared<Uniform_Metal>(
                display::Colour(0.5, 0.5, 0.5), 1., 0.01, 0.01)),
            1.3));
        s.add_light(
            std::make_shared<Point_Light>(structures::Vec3({ 0, -1, 1 }), 1.));

        return s;
    }
} // namespace environment
