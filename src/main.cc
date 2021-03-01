#include <fstream>
#include <memory>

#include "image.hh"
#include "matrix.hh"
#include "scene.hh"

int main()
{
    // Image
    auto file = std::ofstream("test.ppm");
    const double aspect_ratio = 16. / 9.;
    const size_t width = 600;
    const size_t height = width / aspect_ratio;
    auto im = display::Image(height, width);
    size_t sample_per_pixel = 10;

    // Camera
    auto cam_origin = structures::Vec3({ { 0, 0, 0 } });
    auto v_fov = 2.0;
    auto h_fov = aspect_ratio * v_fov;
    auto cam = environment::Camera(
        cam_origin, structures::Vec3({ { 0, 0, 1 } }),
        structures::Vec3({ { 0, -1, 0 } }), 1., v_fov, h_fov);

    // Objects and Lights
    std::vector<std::shared_ptr<environment::Object>> objects;
    std::vector<std::shared_ptr<environment::Light>> lights;
    objects.emplace_back(
        std::make_shared<environment::Sphere>(environment::Sphere(
            structures::Vec3({ { 0, -2, 0 } }),
            new environment::Uniform_Texture(display::Colour(0.25, 0.25, 0.25),
                                             0.7, 0.4, 0.3),
            0.5)));
    objects.emplace_back(std::make_shared<environment::Sphere>(
        environment::Sphere(structures::Vec3({ { 0, -1, -100.5 } }),
                            new environment::Uniform_Texture(
                                display::Colour(0, 0.33, 0.1), 0.9, 0.2, 0.2),
                            100.)));
    objects.emplace_back(std::make_shared<environment::Sphere>(
        environment::Sphere(structures::Vec3({ { 1, -1, 0 } }),
                            new environment::Uniform_Texture(
                                display::Colour(0.8, 0.3, 0.5), 0.5, 0.9, 0.9),
                            0.5)));
    // lights.emplace_back(new environment::Point_Light(
    //    structures::Vec3({ { 0.5, -1.5, 0 } }), 1.));
    lights.emplace_back(std::make_shared<environment::Point_Light>(
        environment::Point_Light(structures::Vec3({ { 0, -2, 1 } }), 3.)));
    // lights.emplace_back(
    //   new environment::Point_Light(structures::Vec3({ { 3, -6, 0 } }), 1.));

    auto scene = environment::Scene(cam, objects, lights, 0.3);

    for (size_t i = 0; i < height; ++i)
        for (size_t j = 0; j < width; ++j)
        {
            auto color = display::Colour();
            for (size_t s = 0; s < sample_per_pixel; ++s)
            {
                auto ray = cam.create_ray(
                    (static_cast<double>(i) + utils::random_double())
                        / (height - 1),
                    (static_cast<double>(j) + utils::random_double())
                        / (width - 1));
                color += scene.cast_ray(ray);
            }

            im.set_pixel(i, j, color / sample_per_pixel);
        }

    file << im;
    return 0;
}
