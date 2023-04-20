#ifndef DSU_H_
#define DSU_H_

#include <array>
#include <stack>
#include <vector>

template <bool Compress = true, bool SupportUndo = false>
struct Dsu {
  static_assert(!Compress || (Compress && !SupportUndo));
  std::vector<int> fa;
  int n, cnt;
  std::stack<std::array<int, 4>> undo;
  Dsu(int n_) : n(n_) {
    fa.resize(n);
    for (int i = 0; i < n; i++) fa[i] = -1;
    cnt = n;
  }
  int Find(int x) {
    assert(0 <= x && x < n);
    if (fa[x] < 0) return x;
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
    assert(x != y);  // Must be two different sets.
    if (fa[x] < fa[y]) std::swap(x, y);
    if (SupportUndo) {
      undo.push({x, fa[x], y, fa[y]});
    }
    fa[y] += fa[x];
    fa[x] = y;
    cnt--;
  }
  std::array<int, 4> Undo() {
    static_assert(SupportUndo);
    assert(!undo.empty());
    auto rec = undo.top();
    auto [x, fax, y, fay] = rec;
    undo.pop();
    fa[x] = fax;
    fa[y] = fay;
    cnt++;
    return rec;
  }
};

template <bool Compress = true, bool SupportUndo = false,
          typename Relation = int, typename Plus = std::plus<Relation>>
struct DsuWithRelation {
  static_assert(!Compress || (Compress && !SupportUndo));
  std::vector<int> fa;
  int n, cnt;
  std::stack<std::function<void()>> undo;
  std::vector<Relation> rel;
  Plus plus;
  DsuWithRelation(int n_, Plus plus_) : n(n_), plus(plus_) {
    fa.resize(n);
    rel.resize(n);
    for (int i = 0; i < n; i++) fa[i] = -1;
    cnt = n;
  }
  // return pair{root, relation of x -> root}
  std::pair<int, Relation> Find(int x) {
    assert(0 <= x && x < n);
    if (fa[x] < 0) return {x, rel[x]};
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
    assert(x != y);  // Must be two different sets.
    if (fa[x] < fa[y]) {
      std::swap(x, y);
      std::swap(relx, rely);
    }
    if (SupportUndo) {
      int fax = fa[x], fay = fa[y];
      undo.push([this, x, y, fax, fay]() {
        fa[x] = fax;
        fa[y] = fa[x];
        rel[x] = {};
        cnt++;
      });
    }
    fa[y] += fa[x];
    fa[x] = y;
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