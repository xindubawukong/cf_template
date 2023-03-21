#include "mint.h"

#include "gtest/gtest.h"

TEST(MIntTest, BasicTest) {
  MInt<29> a = 35;
  EXPECT_EQ(6, a.x);
  auto b = a;
  b *= 2;
  EXPECT_EQ(12, b.x);
  b += a * 4;
  EXPECT_EQ(7, b.x);
  b *= b.Inverse();
  EXPECT_EQ(1, b.x);
  a /= 5;
  EXPECT_EQ(7, a.x);
}