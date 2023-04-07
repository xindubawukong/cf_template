#ifndef DSU_H_
#define DSU_H_

#include <stack>
#include <vector>

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

template <bool Compress = true, bool SupportUndo = false,
          typename Relation = int, typename Plus = std::plus<Relation>>
struct DsuWithRelation {
  static_assert(!Compress || (Compress && !SupportUndo));
  std::vector<int> fa, size;
  int n, cnt;
  std::stack<std::function<void()>> undo;
  std::vector<Relation> rel;
  Plus plus;
  DsuWithRelation(int n_, Plus plus_) : n(n_), plus(plus_) {
    fa.resize(n);
    size.resize(n);
    rel.resize(n);
    for (int i = 0; i < n; i++) {
      fa[i] = i;
      size[i] = 1;
    }
    cnt = n;
  }
  // return pair{root, relation of x -> root}
  std::pair<int, Relation> Find(int x) {
    assert(0 <= x && x < n);
    if (fa[x] == x) return {x, rel[x]};
    auto [root, relfa] = Find(fa[x]);
    if (Compress) {
      fa[x] = root;
      rel[x] = plus(rel[x], relfa);
      return {root, rel[x]};
    } else {
      return {root, plus(rel[x], relfa)};
    }
  }
  void Unite(int x, int y, Relation relx, Relation rely) {
    assert(0 <= x && x < n && 0 <= y && y < n);
    assert(Find(x).first == x && Find(y).first == y);  // Must unite two roots.
    if (x == y) return;
    if (size[x] > size[y]) {
      std::swap(x, y);
      std::swap(relx, rely);
    }
    if (SupportUndo) {
      int fax = fa[x], sizey = size[y];
      undo.push([this, x, y, fax, sizey]() {
        fa[x] = fax;
        size[y] = sizey;
        rel[x] = {};
        cnt++;
      });
    }
    fa[x] = y;
    size[y] += size[x];
    rel[x] = relx;
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