#pragma once

#include <array>
#include <iostream>
#include <math.h>

#include "utils.hh"

namespace structures
{
    template <typename T, const size_t H, const size_t W>
    class FixedMatrix;

    typedef FixedMatrix<double, 1, 3> Vec3;

    template <typename T, const size_t H, const size_t W>
    class FixedMatrix
    {
    public:
        // Constructors
        FixedMatrix() = default;
        FixedMatrix(const FixedMatrix<T, H, W> &m)
        {
            std::copy(m.m().begin(), m.m().end(), m_.begin());
        };
        FixedMatrix(const T (&arr)[H][W])
        {
            for (size_t i = 0; i < H; ++i)
            {
                std::copy(arr[i], arr[i] + W, m_.begin() + i * W);
            }
        }
        FixedMatrix<T, H, W> &operator=(const FixedMatrix<T, H, W> &m)
        {
            return FixedMatrix<T, H, W>(m);
        }

        static constexpr FixedMatrix<T, H, W> identity()
        {
            FixedMatrix<T, H, W> res;
            for (size_t i = 0; i < H; ++i)
                for (size_t j = 0; j < W; ++j)
                    res.at(i, j) = i == j ? 1 : 0;
            return res;
        }

        static constexpr FixedMatrix<T, H, W> arange()
        {
            FixedMatrix<T, H, W> res;
            for (size_t i = 0; i < H; ++i)
                for (size_t j = 0; j < W; ++j)
                    res.at(i, j) = j + i * W;
            return res;
        }

        // Getters
        const std::array<T, (H * W)> &m() const
        {
            return m_;
        }
        const size_t &width()
        {
            return width_;
        }
        const size_t &height()
        {
            return height_;
        }

        // Content Access
        const T &operator[](size_t i) const
        {
            return m_[i];
        }
        T &operator[](size_t i)
        {
            return m_[i];
        }
        const T &at(size_t i, size_t j) const
        {
            return m_[j + i * W];
        }
        T &at(size_t i, size_t j)
        {
            return m_[j + i * W];
        }

        // Matrix operation
        FixedMatrix<T, H, W> operator-() const
        {
            FixedMatrix<T, W, H> opposite;
            for (size_t i = 0; i < H; ++i)
                for (size_t j = 0; j < W; ++j)
                    opposite.at(i, j) = -(this->at(i, j));
            return opposite;
        }

        FixedMatrix<T, W, H> transpose() const
        {
            FixedMatrix<T, W, H> transposed;
            for (size_t i = 0; i < W; ++i)
                for (size_t j = 0; j < H; ++j)
                    transposed.at(i, j) = this->at(j, i);
            return transposed;
        }

        FixedMatrix<T, H, W> &operator*=(const T &rhs)
        {
            for (size_t i = 0; i < H; ++i)
                for (size_t j = 0; j < W; ++j)
                    this->at(i, j) *= rhs;
            return *this;
        }

        FixedMatrix<T, H, W> &operator/=(const T &rhs)
        {
            for (size_t i = 0; i < H; ++i)
                for (size_t j = 0; j < W; ++j)
                    this->at(i, j) /= rhs;
            return *this;
        }

        // Operations between matrices
        FixedMatrix<T, H, W> &operator+=(const FixedMatrix<T, H, W> &rhs)
        {
            for (size_t i = 0; i < H; ++i)
                for (size_t j = 0; j < W; ++j)
                    this->at(i, j) += rhs.at(i, j);
            return *this;
        }
        FixedMatrix<T, H, W> &operator-=(const FixedMatrix<T, H, W> &rhs)
        {
            for (size_t i = 0; i < H; ++i)
                for (size_t j = 0; j < W; ++j)
                    this->at(i, j) -= rhs.at(i, j);
            return *this;
        }

        bool operator==(const FixedMatrix<T, H, W> &m2) const
        {
            auto m1 = *this;
            for (size_t i = 0; i < H; ++i)
                for (size_t j = 0; j < W; ++j)
                    if (!(utils::almost_equal(m1.at(i, j), m2.at(i, j))))
                        return false;
            return true;
        }
        bool operator!=(const FixedMatrix<T, H, W> &m2) const
        {
            return !(*this == m2);
        }

        FixedMatrix<T, H, W> operator+(FixedMatrix<T, H, W> m2) const
        {
            m2 += *this;
            return m2;
        }

        FixedMatrix<T, H, W> operator-(FixedMatrix<T, H, W> m2) const
        {
            auto cpy = *this;
            cpy -= m2;
            return cpy;
        }

        FixedMatrix<T, H, W> operator*(const T &rhs) const
        {
            return rhs * *this;
        }

        FixedMatrix<T, H, W> operator/(const T &rhs) const
        {
            auto copy = FixedMatrix<T, H, W>(*this);
            return copy /= rhs;
        }

        template <const size_t K>
        FixedMatrix<T, H, K> operator*(const FixedMatrix<T, W, K> &rhs) const
        {
            FixedMatrix<T, H, K> res;
            for (size_t i = 0; i < H; ++i)
                for (size_t k = 0; k < K; ++k)
                    for (size_t j = 0; j < W; ++j)
                        res.at(i, k) += this->at(i, j) * rhs.at(j, k);
            return res;
        }

    protected:
        const size_t width_ = W;
        const size_t height_ = H;
        std::array<T, (H * W)> m_ = {};
    };

    template <typename T, const size_t H, const size_t W>
    std::ostream &operator<<(std::ostream &os, const FixedMatrix<T, H, W> &m)
    {
        for (size_t i = 0; i < H; ++i)
        {
            for (size_t j = 0; j < W; ++j)
                os << m.at(i, j) << ' ';
            os << '\n';
        }
        return os;
    }

    template <typename T, const size_t H, const size_t W>
    FixedMatrix<T, H, W> operator*(const T &lhs, FixedMatrix<T, H, W> rhs)
    {
        rhs *= lhs;
        return rhs;
    }

    template <typename T>
    FixedMatrix<T, 1, 3> operator^(const FixedMatrix<T, 1, 3> &lhs,
                                   const FixedMatrix<T, 1, 3> &rhs)
    {
        return FixedMatrix<T, 1, 3>({ { lhs[1] * rhs[2] - lhs[2] * rhs[1],
                                        lhs[2] * rhs[0] - lhs[0] * rhs[2],
                                        lhs[0] * rhs[1] - lhs[1] * rhs[0] } });
    }

    template <typename T, const size_t W>
    double norm(const FixedMatrix<T, 1, W> &v)
    {
        return sqrt((v * v.transpose())[0]);
    }

    template <typename T, const size_t W>
    FixedMatrix<T, 1, W> unit(const FixedMatrix<T, 1, W> &v)
    {
        return v / norm(v);
    }

} // namespace structures
