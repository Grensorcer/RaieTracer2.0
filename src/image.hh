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
        Colour(const double (&rgb)[3])
        {
            std::copy(rgb, rgb + 3, rgb_);
        }
        Colour(double r, double g, double b)
        {
            rgb_[0] = r;
            rgb_[1] = g;
            rgb_[2] = b;
        }

        const double &r() const
        {
            return rgb_[0];
        }
        const double &g() const
        {
            return rgb_[1];
        }
        const double &b() const
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

        Colour &operator+=(const Colour &rhs)
        {
            rgb_[0] += rhs.r();
            rgb_[1] += rhs.g();
            rgb_[2] += rhs.b();
            return *this;
        }

        Colour &operator*=(const Colour &rhs)
        {
            rgb_[0] *= rhs.r();
            rgb_[1] *= rhs.g();
            rgb_[2] *= rhs.b();
            return *this;
        }

        Colour &operator*=(double i)
        {
            rgb_[0] *= i;
            rgb_[1] *= i;
            rgb_[2] *= i;
            return *this;
        }
        Colour &operator/=(double i)
        {
            rgb_[0] /= i;
            rgb_[1] /= i;
            rgb_[2] /= i;
            return *this;
        }

        Colour operator*(double i) const
        {
            auto cpy = *this;
            cpy *= i;
            return cpy;
        }

        Colour operator/(double i) const
        {
            auto cpy = *this;
            cpy /= i;
            return cpy;
        }

        Colour operator+(const Colour &c) const
        {
            auto cpy = *this;
            cpy += c;
            return cpy;
        }

        Colour operator*(const Colour &c) const
        {
            auto cpy = *this;
            cpy *= c;
            return cpy;
        }

        friend std::ostream &operator<<(std::ostream &os, const Colour &c);

    protected:
        double rgb_[3] = {};
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
        return os << static_cast<int>(256 * std::clamp(c.r(), 0., 0.999999))
                  << ' '
                  << static_cast<int>(256 * std::clamp(c.g(), 0., 0.999999))
                  << ' '
                  << static_cast<int>(256 * std::clamp(c.b(), 0., 0.999999));
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
