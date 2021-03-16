#include <chrono>
#include <fstream>
#include <memory>
#include <thread>

#include "image.hh"
#include "matrix.hh"
#include "scene.hh"
#include "utils.hh"

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    // Image
    auto file = std::ofstream("test.ppm");
    const double aspect_ratio = 16. / 9.;
    const size_t width = 900;
    const size_t height = width / aspect_ratio;
    auto im = display::Image(height, width);
    constexpr size_t sample_per_pixel = 4;
    constexpr size_t nb_threads = 8;

    // Camera
    auto cam_origin = structures::Vec3({ { 0, 1, 0.5 } });
    auto v_fov = 2.0;
    auto h_fov = aspect_ratio * v_fov;
    auto cam = environment::Camera(
        cam_origin, structures::Vec3({ { 0, 0, 1 } }),
        structures::Vec3({ { 0, -1, 0 } }), 1., v_fov, h_fov);

    // Objects and Lights
    std::vector<std::shared_ptr<environment::Object>> objects;
    std::vector<std::shared_ptr<environment::Light>> lights;
    objects.emplace_back(std::make_shared<environment::Sphere>(
        structures::Vec3({ { -2, -3, -1 } }),
        new environment::Uniform_Metal(display::Colour(0.25, 0.25, 0.25), 0.8,
                                       1, 1),
        0.5));
    objects.emplace_back(std::make_shared<environment::Sphere>(
        structures::Vec3({ { 0, -3, -1 } }),
        new environment::Uniform_Matte(display::Colour(0., 0., 1.), 0.8, 0.5,
                                       2),
        1.));
    objects.emplace_back(std::make_shared<environment::Sphere>(
        structures::Vec3({ { 2, -3, -1 } }),
        new environment::Uniform_Metal(display::Colour(1., 1., 0.), 0.5, 1.,
                                       2.),
        2.));
    /*
    objects.emplace_back(std::make_shared<environment::Sphere>(
        structures::Vec3({ { 0, -1, -101.5 } }),
        new environment::Uniform_Matte(display::Colour(0, 0.33, 0.1), 1., 1.,
                                       1.),
        100.));
    */
    objects.emplace_back(std::make_shared<environment::Plane>(
        structures::Vec3({ { 0, 0, -1 } }),
        new environment::Uniform_Metal(display::Colour(0, 0.33, 0.1), 1., 1.,
                                       1.),
        structures::Vec3({ { 0, 0, 1 } })));

    objects.emplace_back(std::make_shared<environment::Triangle>(
        new environment::Uniform_Matte(display::Colour(0.80, 0.1, 0.5), 1., 1.,
                                       1.),
        structures::Vec3({ { -2, -3, 1 } }),
        structures::Vec3({ { -1, -5, -1 } }),
        structures::Vec3({ { -3, -5, -1 } })));

    lights.emplace_back(std::make_shared<environment::Point_Light>(
        structures::Vec3({ { -3, -3, 2 } }), 3.));

    auto scene = environment::Scene(cam, objects, lights, 0.3);
    auto t_pool = std::vector<std::thread>();
    auto l = [&](size_t ib, size_t ie) {
        for (size_t i = ib; i < ie; ++i)
        {
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
                    color += scene.cast_ray(ray, 10);
                }

                im.set_pixel(i, j, color / sample_per_pixel);
            }
        }
    };
    for (size_t i = 0; i < nb_threads; ++i)
    {
        // std::cout << "FROM " << i * height / nb_threads << " TO "
        //         << (i + 1) * height / nb_threads << "\n";
        t_pool.emplace_back(std::thread(l, i * height / nb_threads,
                                        (i + 1) * height / nb_threads));
    }
    for (auto &t : t_pool)
        t.join();

    auto stop = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Time taken: " << time.count() << " seconds" << '\n';

    file << im;
    return 0;
}
