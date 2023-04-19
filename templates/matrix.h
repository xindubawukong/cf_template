#ifndef MATRIX_H_
#define MATRIX_H_

#include <vector>

template <typename T>
struct TMatrix {
  int n, m;
  std::vector<std::vector<T>> a;
  TMatrix(int n_, int m_) : n(n_), m(m_) {
    assert(n > 0 && m > 0);
    a.resize(n);
    for (int i = 0; i < n; i++) a[i].resize(m);
  }
  std::vector<T>& operator[](int i) { return a[i]; }
  TMatrix<T> operator*(auto&& b) {
    auto& a = *this;
    assert(a.m == b.n);
    TMatrix<T> c(a.n, b.m);
    for (int k = 0; k < a.m; k++) {
      for (int i = 0; i < a.n; i++) {
        for (int j = 0; j < b.m; j++) {
          c[i][j] += a[i][k] * b[k][j];
        }
      }
    }
    return c;
  }
  TMatrix<T> Transpose() const {
    TMatrix<T> b(m, n);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        b[j][i] = a[i][j];
      }
    }
    return b;
  }
};

#endif  // MATRIX_H_
