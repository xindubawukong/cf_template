#ifndef DSU_H_
#define DSU_H_

#include <vector>

class Dsu {
 public:
  std::vector<int> fa;
  int n, cnt;
  Dsu(int n) : n(n) {
    fa.resize(n);
    for (int i = 0; i < n; i++) fa[i] = i;
    cnt = n;
  }
  int Find(int x) {
    if (fa[x] == x) return x;
    return fa[x] = Find(fa[x]);
  }
  void Unite(int x, int y) {
    x = Find(x);
    y = Find(y);
    if (x != y) {
      fa[y] = x;
      cnt--;
    }
  }
};

#endif  // DSU_H_