#ifndef DSU_H_
#define DSU_H_

#include <array>
#include <cassert>
#include <functional>
#include <stack>
#include <vector>

struct Dsu {
  std::vector<int> fa;
  int n, cc_cnt;
  Dsu(int n_) { Init(n_); }
  void Init(int n_) {
    n = n_;
    fa.resize(n);
    for (int i = 0; i < n; i++) fa[i] = -1;
    cc_cnt = n;
  }
  int Find(int x) {
    assert(0 <= x && x < n);
    if (fa[x] < 0) return x;
    return fa[x] = Find(fa[x]);
  }
  void Unite(int x, int y) {
    assert(0 <= x && x < n && 0 <= y && y < n);
    x = Find(x);
    y = Find(y);
    if (x == y) return;
    if (fa[x] < fa[y]) std::swap(x, y);
    fa[y] += fa[x];
    fa[x] = y;
    cc_cnt--;
  }
};

#endif  // DSU_H_