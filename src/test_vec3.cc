#include <iostream>
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
