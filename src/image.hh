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

    const uint8_t &r() const
    {
        return rgb_[0];
    }
    const uint8_t &g() const
    {
        return rgb_[1];
    }
    const uint8_t &b() const
    {
        return rgb_[2];
    }

    bool operator==(const Colour &c) const
    {
        return this->r() == c.r() && this->g() == c.g() && this->b() == c.b();
    }
    bool operator!=(const Colour &c) const
    {
        return !(*this == c);
    }

protected:
    uint8_t rgb_[3] = {};
};

class Image
{
public:
    using PixelTable = std::vector<Colour>;
    Image() = delete;
    Image(const Image &i) = default;
    Image(size_t height, size_t width)
        : width_{ width }
        , height_{ height }
    {
        pixels_ = std::vector<Colour>(height * width);
    }

    const Colour &get_pixel(size_t i, size_t j) const
    {
        return pixels_[j + i * width_];
    }

    void set_pixel(size_t i, size_t j, const Colour &c)
    {
        pixels_[j + i * width_] = c;
    }

protected:
    size_t width_;
    size_t height_;
    PixelTable pixels_;
};
