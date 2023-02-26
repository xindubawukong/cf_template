#ifndef DSU_H_
#define DSU_H_

#include <vector>

template <typename T = int>
class Dsu {
 public:
  std::vector<int> fa;
  int n, cnt;

  Dsu(int n): n(n) {
    fa.resize(n);
    for (int i = 0; i < n; i++) fa[i] = i;
    cnt = n;
  }

  int GetFa(int x) {
    if (fa[x] == x) return x;
    return fa[x] = GetFa(fa[x]);
  }

  void Merge(int x, int y) {
    x = GetFa(x);
    y = GetFa(y);
    if (x != y) {
      fa[y] = x;
      cnt--;
    }
  }

  std::vector<int> GetCounts() {
    std::vector<int> num(n);
    for (int i = 0; i < n; i++) {
      num[GetFa(i)]++;
    }
    return num;
  }
};

#endif  // DSU_H_