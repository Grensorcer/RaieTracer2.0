#include <fstream>
#include <memory>

#include "image.hh"
#include "matrix.hh"
#include "scene.hh"
#include "utils.hh"

int main()
{
    // Image
    auto file = std::ofstream("test.ppm");
    const double aspect_ratio = 16. / 9.;
    const size_t width = 900;
    const size_t height = width / aspect_ratio;
    auto im = display::Image(height, width);
    size_t sample_per_pixel = 5;

    // Camera
    auto cam_origin = structures::Vec3({ { 0, 0.5, 0 } });
    auto v_fov = 2.0;
    auto h_fov = aspect_ratio * v_fov;
    auto cam = environment::Camera(
        cam_origin, structures::Vec3({ { 0, 0, 1 } }),
        structures::Vec3({ { 0, -1, 0 } }), 1., v_fov, h_fov);

    // Objects and Lights
    std::vector<std::shared_ptr<environment::Object>> objects;
    std::vector<std::shared_ptr<environment::Light>> lights;
    objects.emplace_back(std::make_shared<environment::Sphere>(
        structures::Vec3({ { -1, -2, -1 } }),
        new environment::Uniform_Texture(display::Colour(0.25, 0.25, 0.25), 0.8,
                                         0.4, 0.4),
        0.5));
    objects.emplace_back(std::make_shared<environment::Sphere>(
        structures::Vec3({ { -1, -2, 1 } }),
        new environment::Uniform_Texture(display::Colour(0., 0., 1.), 0.8, 0.4,
                                         0.4),
        0.5));
    objects.emplace_back(std::make_shared<environment::Sphere>(
        structures::Vec3({ { 1, -2, 1 } }),
        new environment::Uniform_Texture(display::Colour(1., 1., 1.), 1., 0.,
                                         0.),
        0.5));
    objects.emplace_back(std::make_shared<environment::Sphere>(
        structures::Vec3({ { 1, -2, -1 } }),
        new environment::Uniform_Texture(display::Colour(1., 0., 0.), 0.8, 0.8,
                                         0.8),
        0.5));
    objects.emplace_back(std::make_shared<environment::Sphere>(
        structures::Vec3({ { 0, -3, 0 } }),
        new environment::Uniform_Texture(display::Colour(0.8, 0.3, 0.5), 1., 1.,
                                         1.),
        1.));
    objects.emplace_back(std::make_shared<environment::Sphere>(
        structures::Vec3({ { 0, -1, -101.5 } }),
        new environment::Uniform_Texture(display::Colour(0, 0.33, 0.1), 1., 0.,
                                         0.),
        100.));
    lights.emplace_back(std::make_shared<environment::Point_Light>(
        structures::Vec3({ { 2, -1.5, 2 } }), 1.));

    auto scene = environment::Scene(cam, objects, lights);

    for (size_t i = 0; i < height; ++i)
    {
        std::cout << static_cast<double>(i) * 100 / (height - 1) << "%\r"
                  << std::flush;
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
                color += scene.cast_ray(ray, 5);
            }

            im.set_pixel(i, j, color / sample_per_pixel);
        }
    }

    file << im;
    return 0;
}
