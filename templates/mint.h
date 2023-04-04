#ifndef MINT_H_
#define MINT_H_

#include <cassert>
#include <iostream>

template <typename T>
T QuickPower(T a, long long b) {
  T res = 1;
  while (b) {
    if (b & 1) res = res * a;
    a *= a;
    b = b >> 1;
  }
  return res;
}

template <int P>
struct MInt {
  static_assert(P > 0);
  int x;
  MInt() : x(0) {}
  MInt(auto x_) { x = Adjust(x_ % P); }

  int Adjust(int x) {
    if (x < 0) {
      x += P;
    } else if (x >= P) {
      x -= P;
    }
    return x;
  }

  MInt<P> Inverse() const {
    assert(x > 0);
    return QuickPower(*this, P - 2);
  }

  MInt<P>& operator+=(const MInt<P>& b) {
    x = Adjust(x + b.x);
    return *this;
  }
  MInt<P>& operator-=(const MInt<P>& b) {
    x = Adjust(x - b.x);
    return *this;
  }
  MInt<P>& operator*=(const MInt<P>& b) {
    x = (long long)x * b.x % P;
    return *this;
  }
  MInt<P>& operator/=(const MInt<P>& b) { return (*this) *= b.Inverse(); }
  MInt<P> operator*(const MInt<P>& b) const {
    auto res = *this;
    res *= b;
    return res;
  }
  operator std::string() { return std::to_string(x); }
  friend std::ostream& operator<<(std::ostream& output, MInt<P>& a) {
    output << string(a);
    return output;
  }

  friend std::istream& operator>>(std::istream& input, MInt<P>& a) {
    input >> a.x;
    return input;
  }
};

#endif  // MINT_H_