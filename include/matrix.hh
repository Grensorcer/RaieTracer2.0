#pragma once

#include <array>
#include <iostream>
#include <math.h>

namespace utils
{
    bool almost_equal(double a, double b);
}

namespace structures
{
    template <size_t H, size_t W>
    class FixedMatrix
    {
    public:
        // Constructors
        FixedMatrix() = default;
        FixedMatrix(const FixedMatrix<H, W> &m)
        {
            std::copy(m.m().begin(), m.m().end(), m_.begin());
        };
        FixedMatrix(const double (&arr)[H][W])
        {
            for (size_t i = 0; i < H; ++i)
                std::copy(arr[i], arr[i] + W, m_.begin() + i * W);
        }
        FixedMatrix<H, W> &operator=(const FixedMatrix<H, W> &m)
        {
            std::copy(m.m().begin(), m.m().end(), m_.begin());
            return *this;
        }

        static constexpr FixedMatrix<H, W> identity()
        {
            FixedMatrix<H, W> res;
            for (size_t i = 0; i < H; ++i)
                for (size_t j = 0; j < W; ++j)
                    res.at(i, j) = i == j ? 1 : 0;
            return res;
        }

        static constexpr FixedMatrix<H, W> arange()
        {
            FixedMatrix<H, W> res;
            for (size_t i = 0; i < H; ++i)
                for (size_t j = 0; j < W; ++j)
                    res.at(i, j) = j + i * W;
            return res;
        }

        // Getters
        inline const std::array<double, (H * W)> &m() const
        {
            return m_;
        }
        inline const size_t &width()
        {
            return width_;
        }
        inline const size_t &height()
        {
            return height_;
        }

        // Content Access
        inline const double &operator[](size_t i) const
        {
            return m_[i];
        }
        inline double &operator[](size_t i)
        {
            return m_[i];
        }
        inline const double &at(size_t i, size_t j) const
        {
            return m_[j + i * W];
        }
        inline double &at(size_t i, size_t j)
        {
            return m_[j + i * W];
        }

        // Matrix operation
        FixedMatrix<H, W> operator-() const
        {
            FixedMatrix<H, W> opposite;
            for (size_t i = 0; i < H; ++i)
                for (size_t j = 0; j < W; ++j)
                    opposite.at(i, j) = -(this->at(i, j));
            return opposite;
        }

        FixedMatrix<W, H> transpose() const
        {
            FixedMatrix<W, H> transposed;
            for (size_t i = 0; i < W; ++i)
                for (size_t j = 0; j < H; ++j)
                    transposed.at(i, j) = this->at(j, i);
            return transposed;
        }

        FixedMatrix<H, W> &operator*=(const double &rhs)
        {
            for (size_t i = 0; i < H; ++i)
                for (size_t j = 0; j < W; ++j)
                    this->at(i, j) *= rhs;
            return *this;
        }

        FixedMatrix<H, W> &operator/=(const double &rhs)
        {
            for (size_t i = 0; i < H; ++i)
                for (size_t j = 0; j < W; ++j)
                    this->at(i, j) /= rhs;
            return *this;
        }

        // Operations between matrices
        FixedMatrix<H, W> &operator+=(const FixedMatrix<H, W> &rhs)
        {
            for (size_t i = 0; i < H; ++i)
                for (size_t j = 0; j < W; ++j)
                    this->at(i, j) += rhs.at(i, j);
            return *this;
        }
        FixedMatrix<H, W> &operator-=(const FixedMatrix<H, W> &rhs)
        {
            for (size_t i = 0; i < H; ++i)
                for (size_t j = 0; j < W; ++j)
                    this->at(i, j) -= rhs.at(i, j);
            return *this;
        }

        bool operator==(const FixedMatrix<H, W> &m2) const
        {
            auto m1 = *this;
            for (size_t i = 0; i < H; ++i)
                for (size_t j = 0; j < W; ++j)
                    if (!(utils::almost_equal(m1.at(i, j), m2.at(i, j))))
                        return false;
            return true;
        }
        bool operator!=(const FixedMatrix<H, W> &m2) const
        {
            return !(*this == m2);
        }

        FixedMatrix<H, W> operator+(FixedMatrix<H, W> m2) const
        {
            m2 += *this;
            return m2;
        }

        FixedMatrix<H, W> operator-(FixedMatrix<H, W> m2) const
        {
            auto cpy = *this;
            cpy -= m2;
            return cpy;
        }

        FixedMatrix<H, W> operator*(const double &rhs) const
        {
            return rhs * *this;
        }

        FixedMatrix<H, W> operator/(const double &rhs) const
        {
            auto copy = FixedMatrix<H, W>(*this);
            return copy /= rhs;
        }

        template <size_t K>
        FixedMatrix<H, K> operator*(const FixedMatrix<W, K> &rhs) const
        {
            FixedMatrix<H, K> res;
            for (size_t i = 0; i < H; ++i)
                for (size_t k = 0; k < K; ++k)
                    for (size_t j = 0; j < W; ++j)
                        res.at(i, k) += this->at(i, j) * rhs.at(j, k);
            return res;
        }

        friend FixedMatrix<1ul, 3ul>
        operator^(const FixedMatrix<1ul, 3ul> &lhs,
                  const FixedMatrix<1ul, 3ul> &rhs);

    protected:
        const size_t width_ = W;
        const size_t height_ = H;
        std::array<double, (H * W)> m_ = {};
    };

    using Vec3 = FixedMatrix<1ul, 3ul>;

    template <size_t H, size_t W>
    std::ostream &operator<<(std::ostream &os, const FixedMatrix<H, W> &m)
    {
        for (size_t i = 0; i < H; ++i)
        {
            for (size_t j = 0; j < W; ++j)
                os << m.at(i, j) << ' ';
            os << '\n';
        }
        return os;
    }

    template <size_t H, size_t W>
    FixedMatrix<H, W> operator*(const double &lhs, FixedMatrix<H, W> rhs)
    {
        rhs *= lhs;
        return rhs;
    }

    template <size_t W>
    double operator*(const FixedMatrix<1, W> &lhs, const FixedMatrix<1, W> &rhs)
    {
        double res = 0.;
        for (size_t i = 0; i < W; ++i)
            res += lhs[i] * rhs[i];
        return res;
    }

    template <size_t W>
    double norm_squared(const FixedMatrix<1, W> &v)
    {
        return v * v;
    }

    template <size_t W>
    double norm(const FixedMatrix<1, W> &v)
    {
        return sqrt(norm_squared(v));
    }

    template <size_t W>
    FixedMatrix<1, W> unit(const FixedMatrix<1, W> &v)
    {
        return v / norm(v);
    }

    template <size_t W>
    FixedMatrix<1, W> &unit(FixedMatrix<1, W> &v)
    {
        v /= norm(v);
        return v;
    }

    Vec3 operator^(const Vec3 &lhs, const Vec3 &rhs);
    Vec3 random();
    Vec3 random(double min, double max);
    Vec3 random_unit1();
    Vec3 random_unit2();

} // namespace structures
