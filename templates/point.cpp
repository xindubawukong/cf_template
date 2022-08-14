#if __APPLE__ && __clang__
#define LOCAL
#endif

#include <algorithm>
#include <any>
#include <array>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <iostream>
#include <map>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef LOCAL
#include "../debug.h"
#else
#define debug(...) 19980723
#endif

using namespace std;

using int64 = long long;
#if _WIN64 || __x86_64__
using int128 = __int128_t;
#endif
using uint = unsigned int;
using uint64 = unsigned long long;

mt19937 rng(19980723);
mt19937_64 rng64(19980723);

// --------------------------- xindubawukong ---------------------------

using real = long double;

const real pi = 3.14159265358979;
const real eps = 1e-7;

struct Point {
  real x, y;
  Point(real _x = 0, real _y = 0) : x(_x), y(_y) {}

  bool operator<(const Point& b) const {
    if (abs(x - b.x) > eps) return x < b.x;
    if (abs(y - b.y) > eps) return y < b.y;
    return false;
  };
  bool operator==(const Point& b) const {
    return abs(x - b.x) < eps && abs(y - b.y) < eps;
  }
  bool operator<=(const Point& b) const { return (*this < b) || (*this == b); }
  bool operator>(const Point& b) const { return !(*this <= b); }
  bool operator!=(const Point& b) const { return !(*this == b); }

  real Length() const { return sqrt(x * x + y * y); }
  friend real Dist(const Point& a, const Point& b) {
    auto x = a.x - b.x, y = a.y - b.y;
    return sqrt(x * x + y * y);
  }
  Point operator+(const Point& b) const { return Point(x + b.x, y + b.y); }
  void operator+=(const Point& b) { x += b.x, y += b.y; }
  Point operator-(const Point& b) const { return Point(x - b.x, y - b.y); }
  Point operator*(real t) const { return Point(x * t, y * t); }
  friend Point operator*(real t, const Point& a) { return a * t; }
  Point operator/(real t) const { return Point(x / t, y / t); }
  real operator*(const Point& b) const { return x * b.x + y * b.y; }
  real operator%(const Point& b) const { return x * b.y - y * b.x; }
  // parallel
  bool operator|(const Point& b) const { return abs((*this) % b) < eps; }

  operator string() const {
    return "Point(" + to_string(x) + ", " + to_string(y) + ")";
  }
  friend ostream& operator<<(ostream& os, const Point& c) {
    os << static_cast<string>(c);
    return os;
  }
};

struct Line {
  Point p, u;
  Line(Point p_ = Point(0, 0), Point u_ = Point(1, 0)) : p(p_), u(u_) {
    assert(u.Length() > eps);
  }

  bool Contains(const Point& a) const {
    if (Dist(p, a) < eps) return true;
    return u | (a - p);
  }
};

struct HalfLine {
  Point p, u;
  HalfLine(Point p_ = Point(0, 0), Point u_ = Point(1, 0)) : p(p_), u(u_) {
    assert(u.Length() > eps);
  }

  bool Contains(const Point& a) const {
    if (Dist(p, a) < eps) return true;
    return (a - p) | u && (a - p) * u > -eps;
  }
};

struct Segment {
  Point a, b;
  Segment(Point a_ = Point(0, 0), Point b_ = Point(1, 0)) : a(a_), b(b_) {
    assert(Dist(a, b) > eps);
  }

  operator string() { return "Segment(" + string(a) + ", " + string(b) + ")"; }

  bool Contains(const Point& p) const {
    if (Dist(a, p) < eps || Dist(b, p) < eps) return true;
    return (p - a) | (b - a) && (a - p) * (b - p) < eps;
  }
};

struct Circle {
  Point c;
  real r;
  Circle(Point c_ = Point(), real r_ = 1) : c(c_), r(r_) { assert(r_ > eps); }

  friend bool IsInside(const Point& a, const Circle& c) {
    auto d = Dist(a, c.c);
    return d - c.r < eps;
  }
};

vector<Point> GetIntersection(const Line& l1, const Line& l2) {
  if (l1.u | l2.u) {
    if (l1.Contains(l2.p)) {
      return {l1.p, l1.p + l1.u};
    } else {
      return {};
    }
  }
  real t = (l1.p - l2.p) % l1.u / (l2.u % l1.u);
  return {l2.p + t * l2.u};
}

vector<Point> GetIntersection(const HalfLine& l, const Segment& s) {
  if (l.u | (s.b - s.a)) {
    if (!Line(l.p, l.u).Contains(s.a)) return {};
    bool t1 = l.Contains(s.a), t2 = l.Contains(s.b);
    if (t1 && t2) {
      return {s.a, s.b};
    } else if (t1) {
      if (abs(Dist(l.p, s.a)) < eps) return {l.p};
      return {l.p, s.a};
    } else if (t2) {
      if (abs(Dist(l.p, s.b)) < eps) return {l.p};
      return {l.p, s.b};
    } else {
      return {};
    }
  }
  auto cs = GetIntersection(Line(l.p, l.u), Line(s.a, s.b - s.a));
  assert(cs.size() == 1);
  auto c = cs[0];
  if (l.Contains(c) && s.Contains(c)) return {c};
  return {};
}

vector<Point> GetIntersection(const Segment& s1, const Segment& s2) {
  if ((s1.b - s1.a) | (s2.b - s2.a)) {
    vector<Point> res;
    if (s1.Contains(s2.a)) res.push_back(s2.a);
    if (s1.Contains(s2.b)) res.push_back(s2.b);
    if (s2.Contains(s1.a)) res.push_back(s1.a);
    if (s2.Contains(s1.b)) res.push_back(s1.b);
    for (int i = res.size() - 1; i >= 0; i--) {
      bool in = false;
      for (int j = 0; j < i; j++) {
        if (Dist(res[i], res[j]) < eps) in = true;
      }
      if (in) res.pop_back();
    }
    return res;
  }
  auto cs = GetIntersection(Line(s1.a, s1.b - s1.a), Line(s2.a, s2.b - s2.a));
  assert(cs.size() == 1);
  auto c = cs[0];
  if (s1.Contains(c) && s2.Contains(c)) return {c};
  return {};
}

Point Rotate(const Point& v, real alpha) {
  real c = cos(alpha);
  real s = sin(alpha);
  return Point(v.x * c - v.y * s, v.x * s + v.y * c);
}

// 0 - pi
real GetAngle(const Point& u, const Point& v) {
  real alpha = u * v / u.Length() / v.Length();
  return acos(alpha);
}

// [0, 2pi)
real GetAngleFromX(const Point& u) {
  if (u.y > eps) {
    return GetAngle(u, {1, 0});
  } else if (u.y < -eps) {
    return 2 * pi - GetAngle(u, {1, 0});
  } else {
    if (u.x > eps) return 0;
    return pi;
  }
}

Point GetSymetricPoint(const Point& a, const Line& l) {
  if (l.Contains(a)) return a;
  auto v = Rotate(l.u, pi / 2);
  auto bs = GetIntersection(l, Line(a, v));
  assert(bs.size() == 1);
  auto b = bs[0];
  return b * 2 - a;
}

// 0 - pi
real GetTriangularAngle(real a, real b, real c) {
  real t = (a * a + b * b - c * c) / (2 * a * b);
  if (t < -1) t = -1;
  if (t > 1) t = 1;
  return acos(t);
}

bool IsTriangularSame(const vector<Point>& a, const vector<Point>& b) {
  assert(a.size() == 3 && b.size() == 3);
  vector<real> p = {Dist(a[0], a[1]), Dist(a[1], a[2]), Dist(a[0], a[2])};
  vector<real> q = {Dist(b[0], b[1]), Dist(b[1], b[2]), Dist(b[0], b[2])};
  sort(p.begin(), p.end());
  sort(q.begin(), q.end());
  for (int i = 0; i < 3; i++) {
    if (abs(p[i] - q[i]) > eps) return false;
  }
  return true;
}

struct Polygon {
  vector<Point> ps;  // clock-wise preferred
  Polygon(vector<Point> ps_ = {}) : ps(ps_) {}

  operator string() {
    string s = "Polygon[";
    for (int i = 0; i < ps.size(); i++) {
      if (i > 0) s += ", ";
      s += string(ps[i]);
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

  bool IsClockWise() {
    assert(ps.size() > 0);
    real sum = 0;
    for (int i = 0; i < ps.size(); i++) {
      sum += 0.5 * ps[i] % ps[(i + 1) % ps.size()];
    }
    return sum < eps;
  }

  vector<Segment> GetSegments() const {
    vector<Segment> segs;
    for (int i = 0; i < ps.size(); i++) {
      auto seg = Segment(ps[i], ps[(i + 1) % ps.size()]);
      segs.push_back(seg);
    }
    return segs;
  }

  friend bool IsStrictlyInside(const Point& a, const Polygon& poly) {
    auto segs = poly.GetSegments();
    for (auto& seg : segs) {
      if (seg.Contains(a)) return false;
    }
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
          if (Dist(p, seg.b) > eps) cnt++;
        }
      }
      return cnt & 1;
    }
    assert(false);
  }

  friend bool IsStrictlyInside(const Polygon& a, const Polygon& b) {
    for (auto& p : a.ps) {
      if (!IsStrictlyInside(p, b)) return false;
    }
    for (auto& p : b.ps) {
      if (IsStrictlyInside(p, a)) return false;
    }
    return true;
  }
};

struct ConnectedArea {
  Polygon out;
  vector<Polygon> ins;

  operator string() {
    string s = "ConnectedArea(";
    s += "out: " + string(out);
    s += ", ins: [";
    for (int i = 0; i < ins.size(); i++) {
      if (i > 0) s += ", ";
      s += string(ins[i]);
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

  vector<Segment> GetSegments() {
    vector<Segment> segs = out.GetSegments();
    for (auto& poly : ins) {
      for (auto& seg : poly.GetSegments()) {
        segs.push_back(seg);
      }
    }
    return segs;
  }

  friend bool IsStrictlyInside(const Point& p, const ConnectedArea& area) {
    if (!IsStrictlyInside(p, area.out)) return false;
    for (auto& poly : area.ins) {
      if (IsStrictlyInside(p, poly)) return false;
    }
    return true;
  }
};

void Main() {}

int main() {
  // std::ios::sync_with_stdio(false);
  // std::cin.tie(nullptr);
#ifdef LOCAL
  freopen("../problem_A/A.in", "r", stdin);
  // freopen("../problem_A/output.txt", "w", stdout);
#endif
  Main();
  return 0;
}
