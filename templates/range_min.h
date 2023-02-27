#ifndef RANGE_MIN_H_
#define RANGE_MIN_H_

#include <vector>

template <typename T, typename Less = std::less<T>>
struct RMQ {
  const std::vector<T>& a;
  Less less;
  int n;
  std::vector<int> log;
  std::vector<std::vector<int>> f;

  RMQ(const std::vector<T>& a) : a(a), less(Less()), n(a.size()) {
    log.resize(n + 1);
    log[1] = 0;
    for (int i = 2, x = 0; i <= n; i++) {
      if (i == (i & -i)) {
        log[i] = log[i - 1] + 1;
      } else {
        log[i] = log[i - 1];
      }
    }
    f.resize(n);
    for (int i = 0; i < n; i++) {
      f[i].resize(log[n] + 1);
      f[i][0] = i;
    }
    for (int j = 1; j <= log[n]; j++) {
      for (int i = 0; i + (1 << (j - 1)) < n; i++) {
        f[i][j] = MinIndex(f[i][j - 1], f[i + (1 << (j - 1))][j - 1]);
      }
    }
  }

  int MinIndex(int i, int j) { return less(a[i], a[j]) ? i : j; }

  int Query(int l, int r) {
    int len = r - l + 1;
    int t = log[len];
    return MinIndex(f[l][t], f[r - (1 << t) + 1][t]);
  }
};

#endif  // RANGE_MIN_H_
