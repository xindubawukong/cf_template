#ifndef TMATRIX_H_
#define TMATRIX_H_

#include <array>
#include <string>
#include <type_traits>

template <typename T, int n>
struct TMatrix {
  static_assert(n > 0);
  TMatrix(bool identity = false) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) data[i][j] = 0;
    }
    if (identity) {
      for (int i = 0; i < n; i++) data[i][i] = 1;
    }
  }
  TMatrix(const TMatrix& b) = default;
  TMatrix(TMatrix&& b) { data = std::move(b.data); }
  TMatrix& operator=(const TMatrix& b) = default;
  TMatrix& operator=(TMatrix&& b) = default;
  std::array<T, n>& operator[](int i) { return data[i]; }
  const std::array<T, n>& operator[](int i) const { return data[i]; }
  TMatrix<T, n> operator*(const TMatrix<T, n>& b) {
    TMatrix<T, n> c;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
          c[i][j] += data[i][k] * b[k][j];
        }
      }
    }
    return c;
  }
  operator std::string() const {
    std::string res = "\n";
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if constexpr (std::is_integral<T>::value ||
                      std::is_floating_point<T>::value) {
          res += std::to_string(data[i][j]);
        } else {
          res += std::string(data[i][j]);
        }
        res += ' ';
      }
      res += '\n';
    }
    return res;
  }

 private:
  std::array<std::array<T, n>, n> data;
};

#endif  // TMATRIX_H_
