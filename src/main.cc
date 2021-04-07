#include <chrono>
#include <fstream>
#include <memory>
#include <thread>

#include "image.hh"
#include "matrix.hh"
#include "scene.hh"
#include "scene_collection.hh"
#include "utils.hh"

int raytracer()
{
    auto start = std::chrono::high_resolution_clock::now();
    // Image
    const double aspect_ratio = 16. / 9.;
    const size_t width = 1920;
    const size_t height = width / aspect_ratio;
    auto im = display::Image(height, width);
    constexpr size_t sample_per_pixel = 4;
    constexpr size_t nb_threads = 4;

    // Camera
    auto cam_origin = structures::Vec3({ 0, 0, 0 });
    auto v_fov = 2.0;
    auto h_fov = aspect_ratio * v_fov;
    auto cam = environment::Camera(
        cam_origin, structures::Vec3({ { 0, 0, 1 } }),
        structures::Vec3({ { 0, -1, 0 } }), 1., v_fov, h_fov);
    auto scene = environment::Scene(cam, 0.3);

    environment::scene_mesh(scene);
    auto stop_create = std::chrono::high_resolution_clock::now();
    auto time_create =
        std::chrono::duration_cast<std::chrono::seconds>(stop_create - start);
    std::cout << "Time taken to compute scene: " << time_create.count()
              << " seconds" << '\n';
    start = std::chrono::high_resolution_clock::now();

    auto t_pool = std::vector<std::thread>();
    auto l = [&](size_t ib) {
        for (size_t i = ib; i < height; i += nb_threads)
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
                    color += scene.cast_ray(ray, 8);
                }

                im.set_pixel(i, j, color / sample_per_pixel);
            }

            std::cout << "Around " << (double)i * 100. / (double)height
                      << "% done                                    \r"
                      << std::flush;
        }
    };
    for (size_t i = 0; i < nb_threads; ++i)
        t_pool.emplace_back(std::thread(l, i));
    for (auto &t : t_pool)
        t.join();

    auto stop_render = std::chrono::high_resolution_clock::now();
    auto time_render =
        std::chrono::duration_cast<std::chrono::seconds>(stop_render - start);
    std::cout << "Time taken to render: " << time_render.count() << " seconds"
              << '\n';

    auto file = std::ofstream("test.ppm");
    file << im;
    return 0;
}

int main()
{
    return raytracer();
}
