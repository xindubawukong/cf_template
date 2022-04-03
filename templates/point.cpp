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
#include "debug.h"
#endif

using namespace std;

using int64 = long long;
#if _WIN64 || __x86_64__
using int128 = __int128_t;
#endif
using uint = unsigned int;
using uint64 = unsigned long long;

// --------------------------- xindubawukong ---------------------------

using real = long double;

const real pi = 3.14159265358979;
const real eps = 1e-7;

struct Point {
  real x, y;
  Point(real _x = 0, real _y = 0) : x(_x), y(_y) {}
  real Length() const { return sqrt(x * x + y * y); }
  friend real Dist(const Point& a, const Point& b) { return (a - b).Length(); }
  Point operator+(const Point& b) const { return Point(x + b.x, y + b.y); }
  Point operator-(const Point& b) const { return Point(x - b.x, y - b.y); }
  Point operator*(real t) const { return Point(x * t, y * t); }
  friend Point operator*(real t, const Point& a) { return a * t; }
  Point operator/(real t) const { return Point(x / t, y / t); }
  real operator*(const Point& b) const { return x * b.x + y * b.y; }
  real operator%(const Point& b) const { return x * b.y - y * b.x; }
  // parallel
  bool operator|(const Point& b) const { return abs((*this) % b) < eps; }

  friend ostream& operator<<(ostream& os, const Point& c) {
    os << "Point(" + to_string(c.x) + ", " + to_string(c.y) + ")";
    return os;
  }
};

struct Line {
  Point p, u;
  Line(Point p_ = Point(), Point u_ = Point()) : p(p_), u(u_) {
    assert(u.Length() > eps);
  }
};

struct HalfLine {
  Point p, u;
  HalfLine(Point p_ = Point(), Point u_ = Point()) : p(p_), u(u_) {
    assert(u.Length() > eps);
  }

  bool Contains(const Point& a) const {
    if (Dist(p, a) < eps) return true;
    return (a - p) | u && (a - p) * u > -eps;
  }
};

struct Segment {
  Point a, b;
  Segment(Point a_ = Point(), Point b_ = Point()) : a(a_), b(b_) {
    assert(Dist(a, b) > eps);
  }

  bool Contains(const Point& p) const {
    if (Dist(a, p) < eps || Dist(b, p) < eps) return true;
    return (p - a) | (b - a) && (a - p) * (b - p) < eps;
  }
};

Point GetIntersection(const Line& l1, const Line& l2) {
  real t = (l1.p - l2.p) % l1.u / (l2.u % l1.u);
  return l2.p + t * l2.u;
}

optional<Point> GetIntersection(const HalfLine& l, const Segment& s) {
  // cout << "GetIntersection " << l.p << " " << l.u << " " << s.a << " " << s.b
  // << endl;
  if (s.Contains(l.p)) return l.p;
  if (l.u | (s.b - s.a)) {
    if (l.Contains(s.a)) {
      real x = Dist(s.a, l.p), y = Dist(s.b, l.p);
      return x < y ? s.a : s.b;
    } else {
      return {};
    }
  }
  Point c = GetIntersection(Line(l.p, l.u), Line(s.a, s.b - s.a));
  if (l.Contains(c) && s.Contains(c)) return c;
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

int n;
Point A, V, U;
real vz, uz, fd;

void Main() {}

int main() {
#ifdef LOCAL
  freopen("../input.txt", "r", stdin);
  // freopen("../output.txt", "w", stdout);
#endif
  Main();
  return 0;
}
