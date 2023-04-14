#ifndef TPOINT_H_
#define TPOINT_H_

#include <iostream>
#include <string>
#include <type_traits>

template <typename T>
struct TPoint {
  using real = T;
  using dim = std::integral_constant<int, 2>;
  static T eps;

  T x, y;
  TPoint(T _x = 0, T _y = 0) : x(_x), y(_y) {}

  T Length2() const { return x * x + y * y; }
  T Length() const { return sqrt(Length2()); }
  friend T Dist(const TPoint<T>& a, const TPoint<T>& b) {
    auto x = a.x - b.x, y = a.y - b.y;
    return sqrt(x * x + y * y);
  }
  TPoint<T> operator+(const TPoint<T>& b) const {
    return TPoint<T>(x + b.x, y + b.y);
  }
  TPoint<T> operator-(const TPoint<T>& b) const {
    return TPoint<T>(x - b.x, y - b.y);
  }
  TPoint<T> operator*(T t) const { return TPoint<T>(x * t, y * t); }
  friend TPoint<T> operator*(T t, const TPoint<T>& a) { return a * t; }
  TPoint<T> operator/(T t) const { return TPoint<T>(x / t, y / t); }
  T operator*(const TPoint<T>& b) const { return x * b.x + y * b.y; }
  T operator%(const TPoint<T>& b) const { return x * b.y - y * b.x; }
  bool operator|(const TPoint<T>& b) const { return abs((*this) % b) <= eps; }
  operator std::string() const {
    return "Point(" + std::to_string(x) + ", " + std::to_string(y) + ")";
  }
  friend std::ostream& operator<<(std::ostream& output, const TPoint<T>& a) {
    output << static_cast<std::string>(a);
    return output;
  }
};

#endif  // TPOINT_H_
