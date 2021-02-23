#include <fstream>

#include "image.hh"
#include "matrix.hh"

int main()
{
    auto file = std::ofstream("test.ppm");
    size_t s = 300;
    auto im = Image(s + 50, s);
    for (size_t i = 0; i < s; ++i)
    {
        im.set_pixel(i, i, Colour(255, 255, 255));
        im.set_pixel(i + 1, i, Colour(10, 100, 220));
    }
    file << im;
    return 0;
}
