#ifndef INT128_H_
#define INT128_H_

#include <iostream>
#include <string>
#include <vector>

struct uint128 {
  __uint128_t x;
  uint128(__uint128_t x_ = 0) : x(x_) {}
  operator std::string() const {
    if (x == 0) return "0";
    auto y = x;
    std::vector<char> a;
    while (y > 0) {
      a.push_back(y % 10);
      y /= 10;
    }
    reverse(a.begin(), a.end());
    std::string s;
    for (auto t : a) s += '0' + t;
    return s;
  }
};

struct int128 {
  __int128_t x;
  int128(__int128_t x_ = 0) : x(x_) {}
  int128 operator-() const { return -x; }
  int128& operator+=(const int128& b) {
    x += b.x;
    return *this;
  }
  int128 operator+(const int128& b) const {
    auto res = *this;
    return res += b;
  }
  int128& operator-=(const int128& b) {
    x -= b.x;
    return *this;
  }
  int128 operator-(const int128& b) const {
    auto res = *this;
    return res -= b;
  }
  int128& operator*=(const int128& b) {
    x *= b.x;
    return *this;
  }
  int128 operator*(const int128& b) const {
    auto res = *this;
    return res *= b;
  }
  friend int128 operator*(const long long t, const int128& a) { return a * t; }
  int128& operator/=(const int128& b) {
    assert(b.x != 0);
    x /= b.x;
    return *this;
  }
  int128 operator/(const int128& b) const {
    auto res = *this;
    return res /= b;
  }
  int128& operator%=(const int128& b) {
    assert(b.x != 0);
    x %= b.x;
    return *this;
  }
  int128 operator%(const int128& b) const {
    auto res = *this;
    return res %= b;
  }
  bool operator<(const int128& b) const { return x < b.x; }
  bool operator<=(const int128& b) const { return x <= b.x; }
  bool operator>(const int128& b) const { return x > b.x; }
  bool operator>=(const int128& b) const { return x >= b.x; }
  bool operator==(const int128& b) const { return x == b.x; }
  operator std::string() const {
    auto t = x;
    std::string s;
    if (t < 0) s += "-", t *= -1;
    s += std::string(uint128(t));
    return s;
  }
  friend std::ostream& operator<<(std::ostream& output, int128 a) {
    output << std::string(a);
    return output;
  }
  friend std::istream& operator>>(std::istream& input, int128& a) {
    std::string s;
    input >> s;
    int i = 0, sign = 1;
    if (s[0] == '-') sign = -1, i = 1;
    a.x = 0;
    while (i < (int)s.length()) {
      a.x = a.x * 10 + s[i] - '0';
      i++;
    }
    a.x *= sign;
    return input;
  }
};

int128 abs(int128 x) {
  if (x < 0) x *= -1;
  return x;
}

#endif  // INT128_H_
