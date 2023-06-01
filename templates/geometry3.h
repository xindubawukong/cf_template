#ifndef GEOMETRY3_H_
#define GEOMETRY3_H_

#include <cassert>
#include <string>
#include <vector>

/*
using real = long double;
using Point = TPoint<real>;
using geo = Geometry3<Point>;
template <>
const real Point::eps = 1e-7;
*/
template <typename Point>
struct Geometry3 {
  using real = typename Point::real;
  using point_t = Point;
  static_assert(Point::dim::value == 3);
  static constexpr real pi = 3.1415926535897932385;

  struct Line {
    Point p, u;
    Line(Point p_ = Point(0, 0, 0), Point u_ = Point(1, 0, 0)) : p(p_), u(u_) {
      assert(u.Length() > 0);
    }
    operator std::string() const {
      return "Line(p=" + std::string(p) + ", u=" + std::string(u) + ")";
    }
  };

  static Point GetProjectionPoint(const Point& a, const Line& l) {
    Point v = a - l.p;
    Point u = l.u / l.u.Length();
    return l.p + (u * v) * u;
  }

  static std::vector<Point> LineToLineIntersection(const Line& l1,
                                                   const Line& l2) {
    assert(!(l1.u | l2.u));
    // solve a1x+b1y=c1, a2x+b2y=c2
    auto Solve = [](real a1, real b1, real c1, real a2, real b2, real c2) {
      real y = (c2 - a2 * c1 / a1) / (b2 - a2 * b1 / a1);
      real x = (c1 - b1 * y) / a1;
      return std::make_pair(x, y);
    };
    auto [k, t] = Solve(l1.u * l1.u, -(l2.u * l1.u), l2.p * l1.u - l1.p * l1.u,
                        l1.u * l2.u, -(l2.u * l2.u), l2.p * l2.u - l1.p * l2.u);
    Point a = l1.p + k * l1.u, b = l2.p + t * l2.u;
    if (Dist(a, b) <= Point::eps) return {a};
    return {a, b};
  }

  struct Segment {
    Point a, b;
    Segment(Point a_ = Point(0, 0, 0), Point b_ = Point(0, 0, 0))
        : a(a_), b(b_) {}
    operator std::string() {
      return "Segment(" + std::string(a) + ", " + std::string(b) + ")";
    }
  };

  static bool IsPointOnSegment(const Point& p, const Segment& seg) {
    if (Dist(p, seg.a) <= Point::eps) return true;
    if (Dist(p, seg.b) <= Point::eps) return true;
    if ((seg.a - p) | (seg.b - p)) {
      return (seg.a - p) * (seg.b - p) <= Point::eps;
    }
    return false;
  }

  struct Sphere {
    Point o;
    real r;
    Sphere(Point o_ = Point(0, 0, 0), real r_ = 1) : o(o_), r(r_) {
      assert(r > Point::eps);
    }
    operator std::string() const {
      return "Sphere(o=" + std::string(o) + ", r=" + std::to_string(r) + ")";
    }
  };

  static std::vector<Point> LineToSphereIntersection(const Line& l,
                                                     const Sphere& s) {
    Point a = GetProjectionPoint(s.o, l);
    real d = Dist(s.o, a);
    if (d - s.r > Point::eps) return {};
    if (abs(d - s.r) <= Point::eps) return {a};
    real t = sqrt(s.r * s.r - d * d);
    Point u = l.u / l.u.Length();
    return {a + t * u, a - t * u};
  }
};

#endif  // GEOMETRY3_H_
