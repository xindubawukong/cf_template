#ifndef TPOINT_H_
#define TPOINT_H_

#include <iostream>
#include <string>
#include <type_traits>

template <typename T>
struct TPoint {
  using real = T;
  using dim = std::integral_constant<int, 2>;
  static const T eps;

  T x, y;
  TPoint(T x_ = 0, T y_ = 0) : x(x_), y(y_) {}
  int Section() const {
    assert(abs(x) > eps || abs(y) > eps);
    if (x >= 0 && y >= 0) return 1;
    if (x <= 0 && y >= 0) return 2;
    if (x <= 0 && y <= 0) return 3;
    if (x >= 0 && y <= 0) return 4;
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
    Out x = a.x - b.x, y = a.y - b.y;
    return x * x + y * y;
  }
  long double Dist(const TPoint<T>& a, const TPoint<T>& b) {
    long double x = a.x - b.x, y = a.y - b.y;
    return sqrt(x * x + y * y);
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
    if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>) {
      return "Point(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    } else {
      return "Point(" + std::string(x) + ", " + std::string(y) + ")";
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

template <typename T>
struct TPoint3 {
  using real = T;
  using dim = std::integral_constant<int, 3>;
  static const T eps;
  T x, y, z;
  TPoint3(T x_ = 0, T y_ = 0, T z_ = 0) : x(x_), y(y_), z(z_) {}

  T Length2() const { return x * x + y * y + z * z; }
  T Length() const { return sqrt(Length2()); }
  friend T Dist(const TPoint3<T>& a, const TPoint3<T>& b) {
    auto x = a.x - b.x, y = a.y - b.y, z = a.z - b.z;
    return sqrt(x * x + y * y + z * z);
  }
  TPoint3<T>& operator+=(const TPoint3<T>& b) {
    x += b.x, y += b.y, z += b.z;
    return *this;
  }
  TPoint3<T> operator+(const TPoint3<T>& b) const {
    auto res = *this;
    return res += b;
  }
  TPoint3<T>& operator-=(const TPoint3<T>& b) {
    x -= b.x, y -= b.y, z -= b.z;
    return *this;
  }
  TPoint3<T> operator-(const TPoint3<T>& b) const {
    auto res = *this;
    return res -= b;
  }
  TPoint3<T>& operator*=(T t) {
    x *= t, y *= t, z *= t;
    return *this;
  }
  TPoint3<T> operator*(T t) const {
    auto res = *this;
    return res *= t;
  }
  friend TPoint3<T> operator*(T t, const TPoint3<T>& a) { return a * t; }
  TPoint3<T>& operator/=(T t) {
    x /= t, y /= t, z /= t;
    return *this;
  }
  TPoint3<T> operator/(T t) const {
    auto res = *this;
    return res /= t;
  }
  T operator*(const TPoint3<T>& b) const { return x * b.x + y * b.y + z * b.z; }
  TPoint3<T> operator%(const TPoint3<T>& b) const {
    return TPoint3<T>(y * b.z - b.y * z, z * b.x - b.z * x, x * b.y - b.x * y);
  }
  bool operator|(const TPoint3<T>& b) const {
    return abs(((*this) % b).Length()) <= eps;
  }
  bool operator<(const TPoint3<T>& b) const {
    if (x != b.x) return x < b.x;
    if (y != b.y) return y < b.y;
    return z < b.z;
  }
  operator std::string() const {
    return "Point(" + std::to_string(x) + ", " + std::to_string(y) + ", " +
           std::to_string(z) + ")";
  }
  friend std::istream& operator>>(std::istream& input, TPoint3<T>& a) {
    input >> a.x >> a.y >> a.z;
    return input;
  }
  friend std::ostream& operator<<(std::ostream& output, const TPoint3<T>& a) {
    output << static_cast<std::string>(a);
    return output;
  }
};

#endif  // TPOINT_H_
