#include <chrono>
#include <fstream>
#include <memory>
#include <thread>

#include "image.hh"
#include "matrix.hh"
#include "scene.hh"
#include "scene_collection.hh"
#include "utils.hh"

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    // Image
    auto file = std::ofstream("test.ppm");
    const double aspect_ratio = 16. / 9.;
    const size_t width = 1920;
    const size_t height = width / aspect_ratio;
    auto im = display::Image(height, width);
    constexpr size_t sample_per_pixel = 10;
    constexpr size_t nb_threads = 8;

    // Camera
    auto cam_origin = structures::Vec3({ { 0, 0, 0 } });
    auto v_fov = 2.0;
    auto h_fov = aspect_ratio * v_fov;
    auto cam = environment::Camera(
        cam_origin, structures::Vec3({ { 0, 0, 1 } }),
        structures::Vec3({ { 0, -1, 0 } }), 1., v_fov, h_fov);
    auto scene = environment::Scene(cam, 0.3);

    environment::scene_triangles(scene);

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
        t_pool.emplace_back(std::thread(l, i * height / nb_threads,
                                        (i + 1) * height / nb_threads));
    for (auto &t : t_pool)
        t.join();

    auto stop = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Time taken: " << time.count() << " seconds" << '\n';

    file << im;
    return 0;
}
