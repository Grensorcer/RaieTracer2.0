#include "image.hh"

#include "utils.hh"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace display
{
    Colour::Colour(const double (&rgb)[3])
    {
        std::copy(rgb, rgb + 3, rgb_);
    }
    Colour::Colour(double r, double g, double b)
    {
        rgb_[0] = r;
        rgb_[1] = g;
        rgb_[2] = b;
    }
    Colour::Colour(const unsigned char *c)
    {
        rgb_[0] = (double)c[0] / 255.;
        rgb_[1] = (double)c[1] / 255.;
        rgb_[2] = (double)c[2] / 255.;
    }

    Colour &Colour::operator=(const Colour &c)
    {
        rgb_[0] = c.r();
        rgb_[1] = c.g();
        rgb_[2] = c.b();
        return *this;
    }

    bool Colour::operator==(const Colour &c) const
    {
        return utils::almost_equal(this->r(), c.r())
            && utils::almost_equal(this->g(), c.g())
            && utils::almost_equal(this->b(), c.b());
    }
    bool Colour::operator!=(const Colour &c) const
    {
        return !(*this == c);
    }

    Colour &Colour::operator+=(const Colour &rhs)
    {
        rgb_[0] += rhs.r();
        rgb_[1] += rhs.g();
        rgb_[2] += rhs.b();
        return *this;
    }

    Colour &Colour::operator*=(const Colour &rhs)
    {
        rgb_[0] *= rhs.r();
        rgb_[1] *= rhs.g();
        rgb_[2] *= rhs.b();
        return *this;
    }

    Colour &Colour::operator*=(double i)
    {
        rgb_[0] *= i;
        rgb_[1] *= i;
        rgb_[2] *= i;
        return *this;
    }
    Colour &Colour::operator/=(double i)
    {
        rgb_[0] /= i;
        rgb_[1] /= i;
        rgb_[2] /= i;
        return *this;
    }

    Colour Colour::operator*(double i) const
    {
        auto cpy = *this;
        cpy *= i;
        return cpy;
    }

    Colour Colour::operator/(double i) const
    {
        auto cpy = *this;
        cpy /= i;
        return cpy;
    }

    Colour Colour::operator+(const Colour &c) const
    {
        auto cpy = *this;
        cpy += c;
        return cpy;
    }

    Colour Colour::operator*(const Colour &c) const
    {
        auto cpy = *this;
        cpy *= c;
        return cpy;
    }

    std::ostream &operator<<(std::ostream &os, const Colour &c)
    {
        return os << static_cast<int>(256 * std::clamp(c.r(), 0., 0.999999))
                  << ' '
                  << static_cast<int>(256 * std::clamp(c.g(), 0., 0.999999))
                  << ' '
                  << static_cast<int>(256 * std::clamp(c.b(), 0., 0.999999));
    }

    const size_t &Image::width() const
    {
        return width_;
    }
    const size_t &Image::height() const
    {
        return height_;
    }

    Image::Image(size_t height, size_t width)
        : width_{ width }
        , height_{ height }
    {
        pixels_ = std::vector<Colour>(height * width);
    }
    Image::Image(const char *file)
    {
        int width, height, n;
        unsigned char *data = stbi_load(file, &width, &height, &n, 3);
        width_ = width;
        height_ = height;
        pixels_.reserve(width * height);
        for (int i = 0; i < 3 * width * height; i += 3)
            pixels_.emplace_back(data + i);
        stbi_image_free(data);
    }

    Image &Image::operator+=(const Image &rhs)
    {
        for (size_t i = 0; i < width_ * height_; ++i)
            pixels_[i] += rhs.pixels_[i];

        return *this;
    }

    Image &Image::operator/=(double d)
    {
        for (size_t i = 0; i < width_ * height_; ++i)
            pixels_[i] /= d;

        return *this;
    }

    std::ostream &operator<<(std::ostream &os, const Image &im)
    {
        os << "P3\n" << im.width_ << ' ' << im.height_ << "\n255\n";

        for (size_t i = 0; i < im.height_; ++i)
            for (size_t j = 0; j < im.width_; ++j)
                os << im.get_pixel(i, j) << '\n';

        return os;
    }

} // namespace display
