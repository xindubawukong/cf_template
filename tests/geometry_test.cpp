#include "geometry.h"

#include "gtest/gtest.h"
#include "tpoint.h"

using Point = TPoint<double>;
using geo = Geometry<Point>;
template <>
const double Point::eps = 1e-6;

TEST(GeometryTest, BasicTest) {
  Point u(1, 0);
  auto v = geo::Rotate(u, 1.3);
  EXPECT_DOUBLE_EQ(1.3, geo::GetAngleFromX(v));
  EXPECT_DOUBLE_EQ(1.3, geo::GetAngle(v, u));

  EXPECT_EQ(geo::pi / 2, geo::GetTriangularAngle(3, 4, 5));
}

TEST(GeometryTest, LineTest) {
  auto line = geo::Line({0, 0}, {1, 2});
  auto p = Point(2, 4);
  EXPECT_TRUE(geo::IsPointOnLine(p, line));
  line = geo::Line({{0, 0}, {1, 0}});
  auto q = geo::GetProjectionPoint(p, line);
  EXPECT_DOUBLE_EQ(q.x, 2);
  EXPECT_DOUBLE_EQ(q.y, 0);

  auto l1 = geo::Line({0, 0}, {1, 1});
  auto l2 = geo::Line({2, 0}, {0, 1});
  p = geo::LineToLineIntersection(l1, l2);
  EXPECT_DOUBLE_EQ(p.x, 2);
  EXPECT_DOUBLE_EQ(p.y, 2);
}
