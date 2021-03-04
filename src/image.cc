#include "image.hh"

#include "utils.hh"

namespace display
{
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

    std::ostream &operator<<(std::ostream &os, const Image &im)
    {
        os << "P3\n" << im.width_ << ' ' << im.height_ << "\n255\n";

        for (size_t i = 0; i < im.height_; ++i)
            for (size_t j = 0; j < im.width_; ++j)
                os << im.get_pixel(i, j) << '\n';

        return os;
    }

} // namespace display
