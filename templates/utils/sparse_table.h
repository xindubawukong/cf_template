#ifndef SPARSE_TABLE_H_
#define SPARSE_TABLE_H_

#include <vector>

template <typename Seq, typename F>
struct SparseTable {
  using T = std::invoke_result_t<F, typename Seq::value_type,
                                 typename Seq::value_type>;
  F f;
  int n;
  std::vector<int> log;
  std::vector<std::vector<T>> table;

  SparseTable(const Seq& a, F f_) : f(f_), n(a.size()) {
    log.resize(n + 1);
    log[1] = 0;
    for (int i = 2; i <= n; i++) {
      if (i == (i & -i)) {
        log[i] = log[i - 1] + 1;
      } else {
        log[i] = log[i - 1];
      }
    }
    table.resize(n);
    for (int i = 0; i < n; i++) {
      table[i].resize(log[n] + 1);
      table[i][0] = a[i];
    }
    for (int j = 1; j <= log[n]; j++) {
      for (int i = 0; i + (1 << (j - 1)) < n; i++) {
        table[i][j] = f(table[i][j - 1], table[i + (1 << (j - 1))][j - 1]);
      }
    }
  }

  T Query(int l, int r) const {
    assert(0 <= l && l <= r && r < n);
    int len = r - l + 1;
    int t = log[len];
    return f(table[l][t], table[r - (1 << t) + 1][t]);
  }
};

#endif  // SPARSE_TABLE_H_
