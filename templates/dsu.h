#ifndef DSU_H_
#define DSU_H_

#include <stack>
#include <vector>

#include "debug.h"

template <bool Compress = true, bool SupportUndo = false>
struct Dsu {
  static_assert(!Compress || (Compress && !SupportUndo));
  std::vector<int> fa, size;
  int n, cnt;
  std::stack<std::function<void()>> undo;
  Dsu(int n_) : n(n_) {
    fa.resize(n);
    size.resize(n);
    for (int i = 0; i < n; i++) {
      fa[i] = i;
      size[i] = 1;
    }
    cnt = n;
  }
  int Find(int x) {
    assert(0 <= x && x < n);
    if (fa[x] == x) return x;
    int root = Find(fa[x]);
    if (Compress) {
      fa[x] = root;
    }
    return root;
  }
  void Unite(int x, int y) {
    assert(0 <= x && x < n && 0 <= y && y < n);
    x = Find(x);
    y = Find(y);
    if (x == y) return;
    if (size[x] > size[y]) std::swap(x, y);
    if (SupportUndo) {
      int fax = fa[x], sizey = size[y];
      undo.push([this, x, y, fax, sizey]() {
        fa[x] = fax;
        size[y] = sizey;
        cnt++;
      });
    }
    fa[x] = y;
    size[y] += size[x];
    cnt--;
  }
  void Undo(int cnt) {
    static_assert(SupportUndo);
    assert(0 <= cnt && cnt <= undo.size());
    for (int i = 0; i < cnt; i++) {
      undo.top()();
      undo.pop();
    }
  }
};

#endif  // DSU_H_