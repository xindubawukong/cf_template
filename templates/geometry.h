#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <algorithm>
#include <cassert>
#include <random>
#include <string>

/*
using real = long double;
using Point = TPoint<real>;
using geo = Geometry<Point>;
template <>
const real Point::eps = 1e-7;

// cout << fixed << setprecision(5) << 1 << '\n';
*/

template <typename Point>
struct Geometry {
  using real = typename Point::real;
  using point_t = Point;
  static_assert(Point::dim::value == 2);

  static constexpr real pi = 3.1415926535897932385;

  static Point Rotate(const Point& v, real alpha) {
    real c = cos(alpha);
    real s = sin(alpha);
    return Point(v.x * c - v.y * s, v.x * s + v.y * c);
  }

  // 0 - pi
  static real GetAngle(const Point& u, const Point& v) {
    auto ul = u.Length(), vl = v.Length();
    if (ul <= Point::eps || vl <= Point::eps) return 0;
    real alpha = u * v / ul / vl;
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
    return abs(l.u % (a - l.p)) <= Point::eps;
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
    if (Dist(l.p, a) <= Point::eps) return true;
    return (a - l.p) | l.u && (a - l.p) * l.u >= -Point::eps;
  }

  struct Segment {
    Point a, b;
    Segment(Point a_ = Point(0, 0), Point b_ = Point(1, 0)) : a(a_), b(b_) {}
    Line ToLine() const { return Line(a, b - a); }
    operator std::string() {
      return "Segment(" + std::string(a) + ", " + std::string(b) + ")";
    }
  };

  static bool IsPointOnSegment(const Point& p, const Segment& s) {
    if (Dist(s.a, p) <= Point::eps || Dist(s.b, p) <= Point::eps) return true;
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
    assert(abs(da - a.r) <= Point::eps);
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
    bool is_convex;
    Polygon(std::vector<Point> ps_ = {}) : ps(ps_), is_convex(false) {}
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
      return sum <= Point::eps;
    }
    bool IsConvex() {
      assert(this->ps.size() >= 2);
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

  static bool IsPointInsidePolygon(const Point& p, const Polygon& poly) {
    if (poly.is_convex) {
      auto& ps = poly.ps;
      auto& o = ps[0];
      Point first = ps[1] - o;
      Point last = ps.back() - o;
      if (first % (p - o) < -Point::eps || last % (p - o) > Point::eps)
        return false;
      auto it = std::lower_bound(
          ps.begin(), ps.end(), p,
          [&](auto& a, auto& b) { return (a - o) % (p - o) >= -Point::eps; });
      int t = it - ps.begin() - 1;
      auto& a = ps[t];
      auto& b = ps[(t + 1) % ps.size()];
      if (IsPointOnSegment(p, {o, a})) return true;
      if (IsPointOnSegment(p, {o, b})) return true;
      if (IsPointOnSegment(p, {a, b})) return true;
      return (p - a) % (b - a) < -Point::eps;
    } else {
      assert(0);  // TBD
    }
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

  static Polygon GetConvex(const std::vector<Point>& ps_) {
    auto ps = ps_;
    assert(ps.size() >= 3);
    int n = ps.size();
    std::sort(ps.begin(), ps.end(), [](auto& a, auto& b) {
      if (abs(a.y - b.y) > Point::eps) return a.y < b.y;
      return a.x > b.x;
    });
    Point o = ps[0];
    std::sort(std::next(ps.begin()), ps.end(), [&](auto& a, auto& b) {
      auto t = (a - o) % (b - o);
      if (abs(t) > Point::eps) return t > 0;
      return Dist(a, o) < Dist(b, o);
    });
    ps.push_back(o);
    std::vector<Point> res = {ps[0], ps[1]};
    for (int i = 2; i <= n; i++) {
      auto& c = ps[i];
      while (res.size() >= 2) {
        auto& a = *std::next(res.rbegin());
        auto& b = *res.rbegin();
        auto u = b - a, v = c - b;
        auto t = u % v;
        if (t < -Point::eps || (abs(t) <= Point::eps && u * v >= -Point::eps)) {
          res.pop_back();
        } else {
          break;
        }
      }
      if (i < n && Dist(c, res.back()) > Point::eps) {
        res.push_back(c);
      }
    }
    Polygon poly(res);
    poly.is_convex = true;
    return poly;
  }

  // Minkowski sum for two convex is also a convex
  static Polygon Minkowski(const Polygon& a, const Polygon& b) {
    assert(a.is_convex && b.is_convex);
    int n = a.ps.size(), m = b.ps.size();
    std::vector<Point> aa(n), bb(m);
    for (int i = 0; i < n; i++) {
      aa[i] = a.ps[(i + 1) % n] - a.ps[i];
    }
    for (int i = 0; i < m; i++) {
      bb[i] = b.ps[(i + 1) % m] - b.ps[i];
    }
    std::vector<Point> all;
    std::merge(aa.begin(), aa.end(), bb.begin(), bb.end(),
               std::back_inserter(all),
               [](auto& u, auto& v) { return u % v > Point::eps; });
    std::vector<Point> res;
    res.push_back(a.ps[0] + b.ps[0]);
    for (auto& u : all) {
      auto p = res.back() + u;
      res.push_back(p);
    }
    assert(Dist(res[0], res.back()) <= Point::eps);
    res.pop_back();
    return GetConvex(res);
  }
};

#endif  // GEOMETRY_H_