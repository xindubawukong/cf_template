#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <string>

/*
using real = long double;
using Point = TPoint<real>;
using geo = Geometry<Point>;
template <>
real Point::eps = 1e-7;

// cout << fixed << setprecision(5) << 1 << '\n';
*/

template <typename Point>
struct Geometry {
  using real = typename Point::real;
  using point_t = Point;

  static constexpr real pi = 3.141592653589793238;

  static Point Rotate(const Point& v, real alpha) {
    real c = cos(alpha);
    real s = sin(alpha);
    return Point(v.x * c - v.y * s, v.x * s + v.y * c);
  }

  // 0 - pi
  static real GetAngle(const Point& u, const Point& v) {
    real alpha = u * v / u.Length() / v.Length();
    return acos(alpha);
  }

  // [0, 2pi)
  static real GetAngleFromX(const Point& P) {
    double ans = atan2(P.y, P.x);
    if (ans < 0.0) ans += 2.0 * pi;
    return ans;
  }

  // 0 - pi
  static real GetTriangularAngle(real a, real b, real c) {
    real t = (a * a + b * b - c * c) / (2 * a * b);
    if (t < -1) t = -1;
    if (t > 1) t = 1;
    return acos(t);
  }

  static bool IsTriangularSame(const std::vector<Point>& a,
                               const std::vector<Point>& b) {
    assert(a.size() == 3 && b.size() == 3);
    std::vector<real> p = {Dist(a[0], a[1]), Dist(a[1], a[2]),
                           Dist(a[0], a[2])};
    std::vector<real> q = {Dist(b[0], b[1]), Dist(b[1], b[2]),
                           Dist(b[0], b[2])};
    sort(p.begin(), p.end());
    sort(q.begin(), q.end());
    for (int i = 0; i < 3; i++) {
      if (abs(p[i] - q[i]) > Point::eps) return false;
    }
    return true;
  }

  struct Line {
    Point p, u;
    Line(Point p_ = Point(0, 0), Point u_ = Point(1, 0)) : p(p_), u(u_) {
      assert(u.Length() > 0);
    }
    operator std::string() const {
      return "Line(p=" + std::string(p) + ", u=" + std::string(u) + ")";
    }
  };

  static bool IsPointOnLine(const Point& a, const Line& l) {
    return abs(l.u % (a - l.p)) < Point::eps;
  }
  static Point GetProjectionPoint(const Point& a, const Line& l) {
    Point v = a - l.p;
    Point u = l.u / l.u.Length();
    return l.p + (u * v) * u;
  }

  static Point LineToLineIntersection(const Line& l1, const Line& l2) {
    assert(!(l1.u | l2.u));
    auto t = (l1.p - l2.p) % l1.u / (l2.u % l1.u);
    return l2.p + t * l2.u;
  }

  struct HalfLine {
    Point p, u;
    HalfLine(Point p_ = Point(0, 0), Point u_ = Point(1, 0)) : p(p_), u(u_) {
      assert(u.Length() > Point::eps);
    }
  };

  static bool IsPointOnHalfLine(const Point& a, const HalfLine& l) {
    if (Dist(l.p, a) < Point::eps) return true;
    return (a - l.p) | l.u && (a - l.p) * l.u > -Point::eps;
  }

  struct Segment {
    Point a, b;
    Segment(Point a_ = Point(0, 0), Point b_ = Point(1, 0)) : a(a_), b(b_) {
      assert(Dist(a, b) > Point::eps);
    }
    Line ToLine() const { return Line(a, b - a); }
    operator std::string() {
      return "Segment(" + std::string(a) + ", " + std::string(b) + ")";
    }
  };

  static bool IsPointOnSegment(const Point& p, const Segment& s) {
    if (Dist(s.a, p) < Point::eps || Dist(s.b, p) < Point::eps) return true;
    return (p - s.a) | (s.b - s.a) && (s.a - p) * (s.b - p) < -Point::eps;
  }

  struct Circle {
    Point c;
    real r;
    Circle(Point c_ = Point(), real r_ = 1) : c(c_), r(r_) {
      assert(r > Point::eps);
    }
    operator std::string() const {
      return "Circle(c=" + std::string(c) + ", r=" + std::to_string(r) + ")";
    }
  };

  static Point Inverse(const Point& o, real r, const Point& a) {
    real da = Dist(o, a);
    assert(da > Point::eps);
    real db = r * r / da;
    return o + (a - o) * db / da;
  }

  static Circle InverseToCircle(const Point& o, real r, const Circle& a) {
    real da = Dist(o, a.c);
    assert(da - a.r > Point::eps);
    real rb = 0.5 * ((1 / (da - a.r)) - (1 / (da + a.r))) * r * r;
    real db = da * rb / a.r;
    real bx = o.x + (a.c.x - o.x) * db / da;
    real by = o.y + (a.c.y - o.y) * db / da;
    return Circle(Point(bx, by), rb);
  }

  static Line InverseToLine(const Point& o, real r, const Circle& a) {
    real da = Dist(o, a.c);
    assert(abs(da - a.r) < Point::eps);
    Point v = a.c - o;
    Point p = o + v * 2;
    Point p_ = Inverse(o, r, p);
    return Line(p_, Rotate(v, pi / 2));
  }

  static Circle InverseToCircle(const Point& o, real r, const Line& l) {
    Point p = GetProjectionPoint(o, l);
    assert(Dist(o, p) > Point::eps);
    Point p_ = Inverse(o, r, p);
    return Circle((o + p_) / 2, Dist(o, p_) / 2);
  }

  struct Polygon {
    std::vector<Point> ps;  // anti-clockwise preferred
    Polygon(std::vector<Point> ps_ = {}) : ps(ps_) {}
    operator std::string() {
      std::string s = "Polygon[";
      for (int i = 0; i < ps.size(); i++) {
        if (i > 0) s += ", ";
        s += std::string(ps[i]);
      }
      s += "]";
      return s;
    }
    real GetArea() const {
      assert(ps.size() > 0);
      real sum = 0;
      for (int i = 0; i < ps.size(); i++) {
        sum += 0.5 * ps[i] % ps[(i + 1) % ps.size()];
      }
      return abs(sum);
    }
    bool IsClockwise() const {
      assert(ps.size() > 0);
      real sum = 0;
      for (int i = 0; i < ps.size(); i++) {
        sum += 0.5 * ps[i] % ps[(i + 1) % ps.size()];
      }
      return sum < Point::eps;
    }
    bool IsConvex() const {
      assert(this->ps.size() > 2);
      auto& ps = this->ps;
      if (this->IsClockwise()) {
        std::reverse(ps.begin(), ps.end());
      }
      int n = ps.size();
      for (int i = 0; i < n; i++) {
        auto& a = ps[i];
        auto& b = ps[(i + 1) % n];
        auto& c = ps[(i + 2) % n];
        auto u = b - a, v = c - a;
        if (u % v < -Point::eps) return false;
      }
      return true;
    }
    std::vector<Segment> Segments() const {
      std::vector<Segment> segs;
      for (int i = 0; i < ps.size(); i++) {
        auto seg = Segment(ps[i], ps[(i + 1) % ps.size()]);
        segs.push_back(seg);
      }
      return segs;
    }
  };

  static bool IsPointStrictlyInsidePolygon(const Point& a,
                                           const Polygon& poly) {
    auto segs = poly.Segments();
    for (auto& seg : segs) {
      if (seg.Contains(a)) return false;
    }
    std::mt19937_64 rng64(0);
    for (int t = 0; t < 10; t++) {
      auto x = 1.0 * (rng64() + 1) / rng64.max();
      auto y = 1.0 * (rng64() + 1) / rng64.max();
      auto u = Point(x, y);
      auto l = HalfLine(a, u);
      bool good = true;
      for (auto& seg : segs) {
        if ((seg.b - seg.a) | u) good = false;
      }
      if (!good) continue;
      int cnt = 0;
      for (auto& seg : segs) {
        auto ips = GetIntersection(l, seg);
        assert(ips.size() <= 1);
        if (ips.size() == 1) {
          auto& p = ips[0];
          if (Dist(p, seg.b) > Point::eps) cnt++;
        }
      }
      return cnt & 1;
    }
    assert(false);
  }

  static bool IsPolygonStrictlyInsidePolygon(const Polygon& a,
                                             const Polygon& b) {
    for (auto& p : a.ps) {
      if (!IsStrictlyInside(p, b)) return false;
    }
    for (auto& p : b.ps) {
      if (IsStrictlyInside(p, a)) return false;
    }
    return true;
  }

  struct ConnectedArea {
    Polygon out;
    std::vector<Polygon> ins;
    operator std::string() {
      std::string s = "ConnectedArea(";
      s += "out: " + std::string(out);
      s += ", ins: [";
      for (int i = 0; i < ins.size(); i++) {
        if (i > 0) s += ", ";
        s += std::string(ins[i]);
      }
      s += "])";
      return s;
    }
    real GetArea() const {
      real res = out.GetArea();
      for (auto& polygon : ins) {
        res -= polygon.GetArea();
      }
      return res;
    }
    std::vector<Segment> Segments() const {
      std::vector<Segment> segs = out.Segments();
      for (auto& poly : ins) {
        for (auto& seg : poly.Segments()) {
          segs.push_back(seg);
        }
      }
      return segs;
    }
  };

  static bool IsPointStrictlyInsideConnectedArea(const Point& p,
                                                 const ConnectedArea& area) {
    if (!IsStrictlyInside(p, area.out)) return false;
    for (auto& poly : area.ins) {
      if (IsStrictlyInside(p, poly)) return false;
    }
    return true;
  }

  static Polygon GetConvex(const std::vector<Point>& ps_) {
    auto ps = ps_;
    assert(ps.size() >= 3);
    int n = ps.size();
    Point O(0, 0);
    for (auto& p : ps) O += p;
    O /= n;
    std::sort(ps.begin(), ps.end(), [&](auto& a, auto& b) {
      auto p = GetAngleFromX(a - O);
      auto q = GetAngleFromX(b - O);
      if (abs(p - q) > Point::eps) return p < q;
      return Dist(a, O) < Dist(b, O);
    });
    auto it = std::min_element(ps.begin(), ps.end(), [](auto& a, auto& b) {
      if (abs(a.x - b.x) > Point::eps) return a.x < b.x;
      return a.y < b.y;
    });
    std::rotate(ps.begin(), it, ps.end());
    ps.push_back(ps[0]);
    Polygon poly;
    poly.ps = {ps[0], ps[1]};
    for (int i = 2; i <= n; i++) {
      auto& c = ps[i];
      if (Dist(c, poly.ps.back()) < Point::eps) continue;
      while (poly.ps.size() >= 2) {
        auto& a = *next(poly.ps.rbegin());
        auto& b = *poly.ps.rbegin();
        if ((c - a) % (b - a) > Point::eps) {
          poly.ps.pop_back();
        } else {
          break;
        }
      }
      if (i < n && Dist(c, poly.ps.back()) > Point::eps) {
        poly.ps.push_back(c);
      }
    }
    return poly;
  }
};

#endif  // GEOMETRY_H_