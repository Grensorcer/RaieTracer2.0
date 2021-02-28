#pragma once

#include <algorithm>
#include <cinttypes>
#include <iostream>
#include <vector>

namespace display
{
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
            return this->r() == c.r() && this->g() == c.g()
                && this->b() == c.b();
        }
        bool operator!=(const Colour &c) const
        {
            return !(*this == c);
        }

        Colour &operator*=(double i)
        {
            rgb_[0] = std::clamp(static_cast<double>(rgb_[0]) * i, 0., 255.);
            rgb_[1] = std::clamp(static_cast<double>(rgb_[1]) * i, 0., 255.);
            rgb_[2] = std::clamp(static_cast<double>(rgb_[2]) * i, 0., 255.);
            return *this;
        }

        Colour operator*(double i) const
        {
            auto cpy = *this;
            cpy *= i;
            return cpy;
        }

        friend std::ostream &operator<<(std::ostream &os, const Colour &c);

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

        friend std::ostream &operator<<(std::ostream &os, const Image &i);

    protected:
        size_t width_;
        size_t height_;
        PixelTable pixels_;
    };

    std::ostream &operator<<(std::ostream &os, const Colour &c)
    {
        return os << +c.r() << ' ' << +c.g() << ' ' << +c.b();
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
