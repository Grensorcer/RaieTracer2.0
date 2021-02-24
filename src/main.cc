#include <fstream>

#include "image.hh"
#include "matrix.hh"
#include "scene.hh"

int main()
{
    auto file = std::ofstream("test.ppm");
    size_t s = 300;
    auto im = display::Image(s + 50, s);
    for (size_t i = 0; i < s; ++i)
    {
        im.set_pixel(i, i, display::Colour(255, 255, 255));
        im.set_pixel(i + 1, i, display::Colour(10, 100, 220));
    }
    file << im;
    return 0;
}
