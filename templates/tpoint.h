#ifndef TPOINT_H_
#define TPOINT_H_

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <type_traits>

template <typename T>
struct TPoint {
  using type = T;
  using dim = std::integral_constant<int, 2>;
  static const T eps;

  T x, y;
  TPoint(T x_ = 0, T y_ = 0) : x(x_), y(y_) {}
  int Section() const {
    assert(abs(x) > eps || abs(y) > eps);
    if (x > 0 && y >= 0) return 1;
    if (x <= 0 && y > 0) return 2;
    if (x < 0 && y <= 0) return 3;
    if (x >= 0 && y < 0) return 4;
    assert(0);
  }
  template <typename Out = T>
  Out Length2() const {
    return (Out)x * x + (Out)y * y;
  }
  long double Length() const {
    return sqrt((long double)x * x + (long double)y * y);
  }
  template <typename Out = T>
  friend Out Dist2(const TPoint<T>& a, const TPoint<T>& b) {
    Out dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
  }
  friend long double Dist(const TPoint<T>& a, const TPoint<T>& b) {
    long double dx = a.x - b.x, dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
  }
  TPoint<T>& operator+=(const TPoint<T>& b) {
    x += b.x, y += b.y;
    return *this;
  }
  TPoint<T> operator+(const TPoint<T>& b) const {
    auto res = *this;
    return res += b;
  }
  TPoint<T>& operator-=(const TPoint<T>& b) {
    x -= b.x, y -= b.y;
    return *this;
  }
  TPoint<T> operator-(const TPoint<T>& b) const {
    auto res = *this;
    return res -= b;
  }
  TPoint<T>& operator*=(T t) {
    x *= t, y *= t;
    return *this;
  }
  TPoint<T> operator*(T t) const {
    auto res = *this;
    return res *= t;
  }
  friend TPoint<T> operator*(T t, const TPoint<T>& a) { return a * t; }
  TPoint<T>& operator/=(T t) {
    x /= t, y /= t;
    return *this;
  }
  TPoint<T> operator/(T t) const {
    auto res = *this;
    return res /= t;
  }
  T operator*(const TPoint<T>& b) const { return x * b.x + y * b.y; }
  T operator%(const TPoint<T>& b) const { return x * b.y - y * b.x; }
  bool operator|(const TPoint<T>& b) const { return abs((*this) % b) <= eps; }
  bool operator<(const TPoint<T>& b) const {
    if (x != b.x) return x < b.x;
    return y < b.y;
  }
  bool operator==(const TPoint<T>& b) const { return x == b.x && y == b.y; }
  operator std::string() const {
    if constexpr (std::is_convertible_v<T, std::string>) {
      return "Point(" + std::string(x) + ", " + std::string(y) + ")";
    } else {
      return "Point(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
  }
  friend std::istream& operator>>(std::istream& input, TPoint<T>& a) {
    input >> a.x >> a.y;
    return input;
  }
  friend std::ostream& operator<<(std::ostream& output, const TPoint<T>& a) {
    output << static_cast<std::string>(a);
    return output;
  }
};

#endif  // TPOINT_H_
