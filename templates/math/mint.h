#ifndef MINT_H_
#define MINT_H_

#include <cassert>
#include <iostream>
#include <vector>

template <typename T>
T Power(T a, long long b) {
  assert(b >= 0);
  T res = 1;
  while (b) {
    if (b & 1) res = res * a;
    a = a * a;
    b = b >> 1;
  }
  return res;
}

template <int P>
struct MInt {
  static_assert(P > 0);
  int x;
  MInt() : x(0) {}
  MInt(int x_) { x = Adjust(x_ % P); }
  MInt(long long x_) { x = Adjust(x_ % P); }

  int Adjust(int t) {
    if (t < 0) {
      t += P;
    } else if (t >= P) {
      t -= P;
    }
    return t;
  }

  MInt<P> Inverse() const {
    assert(x > 0);
    return Power(*this, P - 2);
  }

  bool operator==(const MInt<P>& b) const { return x == b.x; }
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
  MInt<P>& operator/=(const MInt<P>& b) {
    (*this) *= b.Inverse();
    return *this;
  }
  MInt<P> operator+(const MInt<P>& b) const {
    auto res = *this;
    return res += b;
  }
  MInt<P> operator-(const MInt<P>& b) const {
    auto res = *this;
    return res -= b;
  }
  MInt<P> operator*(const MInt<P>& b) const {
    auto res = *this;
    return res *= b;
  }
  MInt<P> operator/(const MInt<P>& b) const {
    auto res = *this;
    return res /= b;
  }
  friend MInt<P> operator*(long long t, const MInt<P>& b) { return b * t; }
  MInt<P> operator-() {
    auto res = *this;
    return res *= -1;
  }
  operator std::string() const { return std::to_string(x); }
  friend std::ostream& operator<<(std::ostream& output, const MInt<P>& a) {
    output << std::string(a);
    return output;
  }
  friend std::istream& operator>>(std::istream& input, MInt<P>& a) {
    input >> a.x;
    return input;
  }
};

template <typename mint>
struct Combination {
  int maxn;
  std::vector<mint> fact, factinv, inv;
  Combination(int maxn_) : maxn(maxn_) {
    fact.resize(maxn + 1);
    factinv.resize(maxn + 1);
    inv.resize(maxn + 1);
    fact[0] = 1;
    for (int i = 1; i <= maxn; i++) {
      fact[i] = fact[i - 1] * i;
    }
    factinv[maxn] = fact[maxn].Inverse();
    for (int i = maxn; i >= 1; i--) {
      inv[i] = factinv[i] * fact[i - 1];
      factinv[i - 1] = factinv[i] * i;
    }
  }
  mint operator()(int n, int m) {
    if (m > n || m < 0) return 0;
    return fact[n] * factinv[n - m] * factinv[m];
  }
};

#endif  // MINT_H_