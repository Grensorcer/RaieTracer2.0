#include <fstream>

#include "image.hh"
#include "matrix.hh"
#include "scene.hh"

int main()
{
    // Image
    auto file = std::ofstream("test.ppm");
    const double aspect_ratio = 16. / 9.;
    const size_t width = 400;
    const size_t height = width / aspect_ratio;
    auto im = display::Image(height, width);

    // Camera
    auto cam_origin = structures::Vec3({ { 0, 0, 0 } });
    auto v_fov = 2.0;
    auto h_fov = aspect_ratio * v_fov;
    auto cam = environment::Camera(
        cam_origin, structures::Vec3({ { 0, 0, 1 } }),
        structures::Vec3({ { 0, -1, 0 } }), 1., v_fov, h_fov);

    // Objects and Lights
    std::vector<environment::Object *> objects;
    std::vector<environment::Light *> lights;
    objects.emplace_back(new environment::Sphere(
        structures::Vec3({ { 0, -1, 0 } }),
        new environment::Uniform_Texture(display::Colour(200, 100, 20), 0.7,
                                         0.4, 0.3),
        0.5));
    objects.emplace_back(
        new environment::Sphere(structures::Vec3({ { 0, -1, -100.5 } }),
                                new environment::Uniform_Texture(
                                    display::Colour(0, 100, 20), 0.3, 0.9, 0.9),
                                100.));
    lights.emplace_back(
        new environment::Point_Light(structures::Vec3({ { 0, 0, 2 } }), 2.));
    // lights.emplace_back(
    //   new environment::Point_Light(structures::Vec3({ { 3, -6, 0 } }), 1.));

    auto scene = environment::Scene(cam, objects, lights, 0.20);

    for (size_t i = 0; i < height; ++i)
        for (size_t j = 0; j < width; ++j)
        {
            auto direction = structures::unit(
                cam.upper_left_corner()
                - static_cast<double>(i) / (height - 1) * cam.z() * cam.v_fov()
                - static_cast<double>(j) / (width - 1) * cam.x() * cam.h_fov());
            auto color = scene.cast_ray(cam.center(), direction);
            im.set_pixel(i, j, color);
        }

    file << im;
    return 0;
}
