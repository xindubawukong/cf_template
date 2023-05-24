#include "tpoint.h"

#include "gtest/gtest.h"

template <>
const int TPoint<int>::eps = 0;
template <>
const double TPoint<double>::eps = 1e-6;

TEST(TPointTest, IntPointTest) {
  using Point = TPoint<int>;
  Point a(1, 1), b(2, 2);
  EXPECT_TRUE(a | b);
  EXPECT_EQ((a - b).Length2(), 2);
}

TEST(TPointTest, DoublePointTest) {
  using Point = TPoint<double>;
  Point a(1, 1), b(2, 2);
  EXPECT_TRUE(a | b);
}
