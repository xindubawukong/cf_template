#ifndef MINT_H_
#define MINT_H_

template <typename T>
T Power(T a, int64 b) {
  T res = 1;
  while (b) {
    if (b & 1) res = res * a;
    a *= a;
    b = b >> 1;
  }
  return res;
}

template <typename T, int P>
struct MInt {
  static_assert(P > 0);
  T x;
  MInt() : x(0) {}
  MInt(auto x_) { x = Adjust(x_ % P); }

  T Adjust(T x) {
    if (x < 0) {
      x += P;
    } else if (x >= P) {
      x -= P;
    }
    return x;
  }

  MInt<T, P> Inverse() {
    assert(x > 0);
    return Power(*this, P - 2);
  }

  MInt<T, P>& operator+=(const MInt<T, P>& b) {
    x = Adjust(x + b.x);
    return *this;
  }
  MInt<T, P>& operator-=(const MInt<T, P>& b) {
    x = Adjust(x - b.x);
    return *this;
  }
  MInt<T, P>& operator*=(const MInt<T, P>& b) {
    x = (int64)x * b.x % P;
    return *this;
  }
  MInt<T, P>& operator/=(const MInt<T, P>& b) { return (*this) *= b.Inverse(); }
  MInt<T, P> operator*(const MInt<T, P>& b) {
    auto res = *this;
    res *= b;
    return res;
  }
};

#endif  // MINT_H_