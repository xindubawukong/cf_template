#ifndef GAUSS_H_
#define GAUSS_H_

#include <vector>

template <typename T>
std::vector<T> Gauss(int n, std::vector<std::vector<T>> a) {
  assert(n > 0 && a.size() == n);
  for (int i = 0; i < n; i++) assert(a[i].size() == n + 1);
  for (int i = 0; i < n; i++) {
    int id = i;
    T val = abs(a[i][i]);
    for (int j = i + 1; j < n; j++) {
      if (abs(a[j][i]) > val) {
        id = j;
        val = abs(a[j][i]);
      }
    }
    swap(a[i], a[id]);
    assert(val > 1e-7);
    for (int j = i + 1; j < n; j++) {
      T t = a[j][i] / a[i][i];
      for (int k = i; k <= n; k++) a[j][k] -= t * a[i][k];
    }
  }
  std::vector<T> x(n);
  x[n - 1] = a[n - 1][n] / a[n - 1][n - 1];
  for (int i = n - 2; i >= 0; i--) {
    T t = 0;
    for (int j = i + 1; j < n; j++) t += a[i][j] * x[j];
    x[i] = (a[i][n] - t) / a[i][i];
  }
  return x;
}

#endif  // GAUSS_H_
