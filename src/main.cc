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
    auto cam = environment::Camera(cam_origin, 1., v_fov, h_fov);

    for (size_t i = 0; i < s; ++i)
    {
        im.set_pixel(i, i, display::Colour(255, 255, 255));
        im.set_pixel(i + 1, i, display::Colour(10, 100, 220));
    }
    file << im;
    return 0;
}
