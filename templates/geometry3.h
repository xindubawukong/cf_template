#ifndef GEOMETRY3_H_
#define GEOMETRY3_H_

#include <cassert>
#include <string>

template <typename Point3>
struct Geometry3 {
  using real = typename Point3::real;
  using point_t = Point3;
  static_assert(Point3::dim::value == 3);
  static constexpr real pi = 3.1415926535897932385;

  struct Line {
    Point3 p, u;
    Line(Point3 p_ = Point3(0, 0, 0), Point3 u_ = Point3(1, 0, 0))
        : p(p_), u(u_) {
      assert(u.Length() > 0);
    }
    operator std::string() const {
      return "Line(p=" + std::string(p) + ", u=" + std::string(u) + ")";
    }
  };

  struct Segment {
    Point3 a, b;
    Segment(Point3 a_ = Point3(0, 0, 0), Point3 b_ = Point3(0, 0, 0))
        : a(a_), b(b_) {}
    operator std::string() {
      return "Segment(" + std::string(a) + ", " + std::string(b) + ")";
    }
  };

  struct Sphere {
    Point3 c;
    real r;
    Sphere(Point3 c_ = Point3(0, 0, 0), real r_ = 1) : c(c_), r(r_) {
      assert(r > Point3::eps);
    }
    operator std::string() const {
      return "Sphere(c=" + std::string(c) + ", r=" + std::to_string(r) + ")";
    }
  };
};

#endif  // GEOMETRY3_H_
