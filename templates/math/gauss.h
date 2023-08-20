#ifndef GAUSS_H_
#define GAUSS_H_

#include <cassert>
#include <vector>

template <typename T>
std::vector<T> Gauss(int n, std::vector<std::vector<T>> a) {
  assert(n > 0 && (int)a.size() == n);
  for (int i = 0; i < n; i++) assert((int)a[i].size() == n + 1);
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
    if (abs(val) < 1e-7) continue;
    for (int j = i + 1; j < n; j++) {
      T t = a[j][i] / a[i][i];
      for (int k = i; k <= n; k++) a[j][k] -= t * a[i][k];
    }
  }
  std::vector<T> x(n);
  for (int i = n - 1; i >= 0; i--) {
    T t = 0;
    for (int j = i + 1; j < n; j++) t += a[i][j] * x[j];
    if (abs(a[i][i]) < 1e-7) continue;
    x[i] = (a[i][n] - t) / a[i][i];
  }
  return x;
}

#endif  // GAUSS_H_
