#include <array>
#include <cstdint>

#include "gtest/gtest.h"
#include "matrix.hh"

using structures::FixedMatrix;

TEST(Matrix, MatrixBuild)
{
    FixedMatrix<int, 3, 3> m;
    std::cout << m;
    ASSERT_TRUE(true);
}

TEST(Matrix, Identity)
{
    auto id = FixedMatrix<int, 3, 3>::identity();
    auto check =
        FixedMatrix<int, 3, 3>({ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } });
    ASSERT_EQ(check, id);
}

TEST(Matrix, MatrixFrom1)
{
    int a[2][3] = { { 1, 2, 3 }, { 3, 4, 5 } };
    auto m = FixedMatrix<int, 2, 3>(a);
    std::cout << m;
    ASSERT_EQ(1, m.at(0, 0));
    ASSERT_EQ(2, m.at(0, 1));
    ASSERT_EQ(3, m.at(0, 2));
    ASSERT_EQ(3, m.at(1, 0));
    ASSERT_EQ(4, m.at(1, 1));
    ASSERT_EQ(5, m.at(1, 2));
    ASSERT_EQ(3, m.width());
    ASSERT_EQ(2, m.height());
}

TEST(Matrix, MatrixFrom2)
{
    auto m = FixedMatrix<int, 2, 2>({ { 1, 2 }, { 3, 4 } });
    std::cout << m;
    ASSERT_EQ(1, m.at(0, 0));
    ASSERT_EQ(2, m.at(0, 1));
    ASSERT_EQ(3, m.at(1, 0));
    ASSERT_EQ(4, m.at(1, 1));
}

TEST(Matrix, MatrixAdd1)
{
    auto m1 = FixedMatrix<int, 1, 3>::arange();
    auto m2 = FixedMatrix<int, 1, 3>::arange();
    auto res = FixedMatrix<int, 1, 3>({ { 0, 2, 4 } });
    ASSERT_EQ(res, m1 + m2);
}

TEST(Matrix, MatrixAdd2)
{
    auto m1 = FixedMatrix<int, 3, 3>::arange();
    auto m2 = FixedMatrix<int, 3, 3>::arange();
    auto res =
        FixedMatrix<int, 3, 3>({ { 0, 2, 4 }, { 6, 8, 10 }, { 12, 14, 16 } });
    ASSERT_EQ(res, m1 + m2);
}

TEST(Matrix, MatrixT)
{
    auto m = FixedMatrix<int, 3, 1>::arange();
    auto res = FixedMatrix<int, 1, 3>({ { 0, 1, 2 } });
    ASSERT_EQ(res, m.transpose());
}

TEST(Matrix, MatrixScalarMul1)
{
    auto m = FixedMatrix<int, 2, 3>::arange();
    auto res = FixedMatrix<int, 2, 3>({ { 0, 3, 6 }, { 9, 12, 15 } });
    ASSERT_EQ(res, 3 * m);
    ASSERT_EQ(res, m * 3);
    m *= 3;
    ASSERT_EQ(m, res);
}

TEST(Matrix, MatrixScalarMul2)
{
    auto m = FixedMatrix<double, 2, 3>::arange();
    auto res = FixedMatrix<double, 2, 3>({ { 0, 0.5, 1 }, { 1.5, 2, 2.5 } });
    ASSERT_EQ(res, 0.5 * m);
    ASSERT_EQ(res, m * 0.5);
    m *= 0.5;
    ASSERT_EQ(m, res);
}

TEST(Matrix, IdentityMul)
{
    auto m1 = FixedMatrix<int, 3, 3>::arange();
    auto m2 = FixedMatrix<int, 3, 3>::identity();
    ASSERT_EQ(m1, m1 * m2);
}

TEST(Matrix, SquareMul)
{
    auto m1 = FixedMatrix<int, 3, 3>::arange();
    auto m2 = FixedMatrix<int, 3, 3>::arange();
    auto res = FixedMatrix<int, 3, 3>(
        { { 15, 18, 21 }, { 42, 54, 66 }, { 69, 90, 111 } });
    ASSERT_EQ(res, m1 * m2);
}

TEST(Matrix, ScalarProduct)
{
    auto m1 = FixedMatrix<int, 1, 4>::arange();
    auto m2 = FixedMatrix<int, 4, 1>::arange();
    auto res = FixedMatrix<int, 1, 1>({ { 14 } });
    ASSERT_EQ(res, m1 * m2);
}

TEST(Matrix, RegularMul)
{
    auto m1 = FixedMatrix<int, 1, 4>::arange();
    auto m2 = FixedMatrix<int, 4, 2>::arange();
    auto res = FixedMatrix<int, 1, 2>({ { 28, 34 } });
    ASSERT_EQ(res, m1 * m2);
}

TEST(Matrix, Minus)
{
    auto m1 = FixedMatrix<int, 3, 3>::arange();
    auto m2 = -m1;
    ASSERT_EQ((FixedMatrix<int, 3, 3>::arange()), m1);
    ASSERT_EQ((FixedMatrix<int, 3, 3>(
                  { { 0, -1, -2 }, { -3, -4, -5 }, { -6, -7, -8 } })),
              m2);
}

TEST(Matrix, Sub)
{
    auto m1 = FixedMatrix<int, 3, 3>::arange();
    auto res = FixedMatrix<int, 3, 3>();
    ASSERT_EQ(res, m1 - m1);
}
