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

        Colour &operator=(const Colour &c);
        bool operator==(const Colour &c) const;
        bool operator!=(const Colour &c) const;

        Colour &operator+=(const Colour &rhs);
        Colour &operator*=(const Colour &rhs);
        Colour &operator*=(double i);
        Colour &operator/=(double i);

        Colour operator*(double i) const;
        Colour operator/(double i) const;
        Colour operator+(const Colour &c) const;
        Colour operator*(const Colour &c) const;

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

        inline const Colour &get_pixel(size_t i, size_t j) const
        {
            return pixels_[j + i * width_];
        }

        inline void set_pixel(size_t i, size_t j, const Colour &c)
        {
            pixels_[j + i * width_] = c;
        }

        Image &operator+=(const Image &im);
        Image &operator/=(double d);
        friend std::ostream &operator<<(std::ostream &os, const Image &i);

    protected:
        size_t width_;
        size_t height_;
        PixelTable pixels_;
    };

    std::ostream &operator<<(std::ostream &os, const Colour &c);
    std::ostream &operator<<(std::ostream &os, const Image &im);
} // namespace display
