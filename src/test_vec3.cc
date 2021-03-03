#include <iostream>
#include <math.h>
#include <typeinfo>

#include "gtest/gtest.h"
#include "matrix.hh"

using structures::FixedMatrix;
using structures::Vec3;

TEST(Vec3, ScalarProduct)
{
    auto v1 = Vec3::arange();
    auto v2 = Vec3::arange();
    auto res = FixedMatrix<1, 1>({ { 5 } });
    auto res2 = v1 * v2.transpose();
    ASSERT_EQ(res, res2);
}

TEST(Vec3, VectorProduct1)
{
    auto v1 = Vec3::arange();
    auto v2 = Vec3::arange();
    auto res = Vec3({ { 0, 0, 0 } });
    ASSERT_EQ(res, v1 ^ v2);
}

TEST(Vec3, VectorProduct2)
{
    auto v1 = Vec3({ { 0, 0, 1 } });
    auto v2 = Vec3({ { 0, -1, 0 } });
    auto res = Vec3({ { 1, 0, 0 } });
    ASSERT_EQ(res, v1 ^ v2);
}

TEST(Vec3, VectorDifference)
{
    auto v1 = Vec3({ { 7, 8, -1 } });
    auto v2 = Vec3({ { 0, 0, 0 } });
    auto v3 = Vec3({ { -10, 1, 1 } });
    ASSERT_EQ(Vec3({ { 7, 8, -1 } }), v1 - v2);
    ASSERT_EQ(Vec3({ { 17, 7, -2 } }), v1 - v3);
    ASSERT_EQ(Vec3({ { -7, -8, 1 } }), v2 - v1);
    ASSERT_EQ(Vec3({ { -10, 1, 1 } }), v3 - v2);
}

TEST(Vec3, Divide)
{
    auto v1 = Vec3::arange();
    double x = 5;
    auto res = Vec3({ { 0, 1. / 5., 2. / 5. } });
    ASSERT_EQ(res, v1 / x);
    ASSERT_EQ(2, v1[2]);
}

TEST(Vec3, Norm)
{
    auto v1 = Vec3::arange();
    ASSERT_TRUE(fabs(structures::norm(v1) - 2.23606797749979) < 0.001);
}

TEST(Vec3, Unit)
{
    auto v1 = Vec3::arange();
    ASSERT_NE(Vec3(), structures::unit(v1));
    ASSERT_EQ(Vec3({ { 0, 0.4472136, 0.89442719 } }), structures::unit(v1));
}
