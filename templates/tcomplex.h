#ifndef TCOMPLEX_H_
#define TCOMPLEX_H_

template <typename T>
struct TComplex {
  T r, i;
  TComplex(T r_ = 0, T i_ = 0) : r(r_), i(i_) {}
  TComplex& operator+=(const TComplex& b) {
    r += b.r;
    i += b.i;
    return *this;
  }
  TComplex operator+(const TComplex& b) const {
    auto res = *this;
    res += b;
    return res;
  }
  TComplex& operator-=(const TComplex& b) {
    r -= b.r;
    i -= b.i;
    return *this;
  }
  TComplex operator-(const TComplex& b) const {
    auto res = *this;
    res -= b;
    return res;
  }
  TComplex& operator*=(const TComplex& b) {
    auto res = *this;
    res = res * b;
    *this = res;
  }
  TComplex operator*(const TComplex& b) const {
    return TComplex(r * b.r - i * b.i, r * b.i + i * b.r);
  }
};

#endif  // TCOMPLEX_H_
