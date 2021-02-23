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
    auto res = FixedMatrix<double, 1, 1>({ { 5 } });
    auto res2 = v1 * v2.transpose();
    ASSERT_EQ(res, res2);
}

TEST(Vec3, VectorProduct)
{
    auto v1 = Vec3::arange();
    auto v2 = Vec3::arange();
    auto res = Vec3({ { 0, 0, 0 } });
    ASSERT_EQ(res, v1 ^ v2);
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
