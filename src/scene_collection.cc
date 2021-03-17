#include "scene_collection.hh"

namespace environment
{
    Scene &scene_one(Scene &s)
    {
        s.add_light(std::make_shared<environment::Point_Light>(
            structures::Vec3({ { -3, -3, 4 } }), 3.));

        s.add_object(std::make_shared<environment::Sphere>(
            structures::Vec3({ { -2, -3, -1 } }),
            new environment::Uniform_Metal(display::Colour(0.25, 0.25, 0.25),
                                           0.8, 1, 1),
            0.5));
        s.add_object(std::make_shared<environment::Sphere>(
            structures::Vec3({ { 0, -3, -1 } }),
            new environment::Uniform_Matte(display::Colour(0., 0., 1.), 0.8,
                                           0.5, 2),
            1.));
        s.add_object(std::make_shared<environment::Sphere>(
            structures::Vec3({ { 2, -3, -1 } }),
            new environment::Uniform_Smooth(display::Colour(1., 1., 0.), 0.5,
                                            1., 2.),
            2.));
        /*
        s.add_object(std::make_shared<environment::Sphere>(
            structures::Vec3({ { 0, -1, -101.5 } }),
            new environment::Uniform_Matte(display::Colour(0, 0.33,
        0.1), 1., 1., 1.), 100.));
        */
        s.add_object(std::make_shared<environment::Plane>(
            structures::Vec3({ { 0, 0, -1 } }),
            new environment::Uniform_Matte(display::Colour(0, 0.33, 0.1), 1.,
                                           1., 1.),
            structures::Vec3({ { 0, 0, 1 } })));

        s.add_object(std::make_shared<environment::Triangle>(
            new environment::Uniform_Metal(display::Colour(1., 1., 1.), 0.01,
                                           0.01, 0.01),
            structures::Vec3({ { -2.5, -4.5, 1 } }),
            structures::Vec3({ { -2, -5, -1 } }),
            structures::Vec3({ { -4, -4, -1 } })));

        return s;
    }

    Scene &scene_triangles(Scene &s)
    {
        for (size_t k = 0; k < 6; ++k)
            for (size_t i = 0; i < 6; ++i)
                for (size_t j = 0; j < 6; ++j)
                    s.add_object(std::make_shared<environment::Triangle>(
                        new environment::Uniform_Smooth(
                            display::Colour(0.2, 0.3, 0.7), 1., 1., 1.),
                        structures::Vec3({ { 2. - i, -(4. + k), 2. - j } }),
                        structures::Vec3({ { 2. - i, -(4. + k), 3. - j } }),
                        structures::Vec3({ { 3. - i, -(4. + k), 2. - j } })));

        s.add_light(std::make_shared<environment::Point_Light>(
            structures::Vec3({ { 0, -5, 5 } }), 5.));

        return s;
    }
} // namespace environment
