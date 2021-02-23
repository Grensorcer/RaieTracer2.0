#include "gtest/gtest.h"
#include "image.hh"

TEST(Colour, GetComponents)
{
    auto c1 = Colour();
    auto c2 = Colour(100, 254, 255);
    auto c3 = Colour({ 10, 20, 30 });

    ASSERT_EQ(0, c1.r());
    ASSERT_EQ(0, c1.g());
    ASSERT_EQ(0, c1.b());

    ASSERT_EQ(100, c2.r());
    ASSERT_EQ(254, c2.g());
    ASSERT_EQ(255, c2.b());

    ASSERT_EQ(10, c3.r());
    ASSERT_EQ(20, c3.g());
    ASSERT_EQ(30, c3.b());
}

TEST(Image, EmptyImage)
{
    auto i = Image(10, 10);
    ASSERT_EQ(Colour(), i.get_pixel(5, 5));
    i.set_pixel(5, 5, Colour(20, 30, 40));
    ASSERT_NE(Colour(), i.get_pixel(5, 5));
}
