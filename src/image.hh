#include <cinttypes>
#include <iostream>
#include <vector>

class Colour
{
public:
    Colour() = default;
    Colour(const Colour &c) = default;
    Colour(const uint8_t (&rgb)[3])
    {
        std::copy(rgb, rgb + 3, rgb_);
    }
    Colour(uint8_t r, uint8_t g, uint8_t b)
    {
        rgb_[0] = r;
        rgb_[1] = g;
        rgb_[2] = b;
    }

    const uint8_t &r()
    {
        return rgb_[0];
    }
    const uint8_t &g()
    {
        return rgb_[1];
    }
    const uint8_t &b()
    {
        return rgb_[2];
    }

protected:
    uint8_t rgb_[3] = {};
};

class Image
{
public:
    using PixelTable = std::vector<Colour>;

private:
    size_t width_;
    size_t height_;
    PixelTable pixels;
};
