#ifndef HEAVY_LIGHT_DECOMPOSITION_H_
#define HEAVY_LIGHT_DECOMPOSITION_H_

#include <cassert>
#include <functional>
#include <vector>

template <typename Graph>
struct HeavyLightDecomposition {
  int n, root;
  Graph* tree;
  std::vector<int> size, dep, pos, which, heavy, fa, up;

  void Init(int n_) {
    n = n_;
    size.resize(n);
    dep.resize(n);
    pos.resize(n);
    which.resize(n);
    heavy.resize(n);
    fa.resize(n);
    up.resize(n);
  }

  HeavyLightDecomposition(Graph& tree_, int root_ = 0)
      : n(tree_.n), tree(&tree_), root(root_) {
    assert(!tree->IsDirected());
    assert((int)tree->edges.size() == n - 1);
    Init(n);

    std::function<void(int)> Dfs1 = [&](int u) {
      heavy[u] = -1;
      size[u] = 1;
      for (int v : tree->Neighbors(u)) {
        if (v == fa[u]) continue;
        fa[v] = u;
        dep[v] = dep[u] + 1;
        Dfs1(v);
        size[u] += size[v];
        if (heavy[u] == -1 || size[v] > size[heavy[u]]) {
          heavy[u] = v;
        }
      }
    };
    dep[root] = 0;
    fa[root] = -1;
    Dfs1(root);

    int cnt = 0;
    std::function<void(int)> Dfs2 = [&](int u) {
      pos[u] = cnt;
      which[cnt] = u;
      cnt++;
      if (heavy[u] == -1) return;
      up[heavy[u]] = up[u];
      Dfs2(heavy[u]);
      for (int v : tree->Neighbors(u)) {
        if (v == fa[u] || v == heavy[u]) continue;
        up[v] = v;
        Dfs2(v);
      }
      assert(cnt == size[u] + pos[u]);
    };
    up[root] = root;
    Dfs2(root);
    assert(cnt == n);
  }

  std::vector<std::pair<int, int>> PathIntervals(int u, int v) {
    assert(0 <= u && u < n && 0 <= v && v < n);
    std::vector<std::pair<int, int>> intervals;
    while (up[u] != up[v]) {
      if (dep[up[u]] < dep[up[v]]) std::swap(u, v);
      intervals.push_back({pos[up[u]], pos[u]});
      u = fa[up[u]];
    }
    if (dep[u] < dep[v]) std::swap(u, v);
    intervals.push_back({pos[v], pos[u]});
    return intervals;
  }

  std::pair<int, int> SubTreeInterval(int u) {
    return {pos[u], pos[u] + size[u] - 1};
  }

  int Lca(int u, int v) {
    assert(0 <= u && u < n && 0 <= v && v < n);
    while (up[u] != up[v]) {
      if (dep[up[u]] < dep[up[v]]) std::swap(u, v);
      u = fa[up[u]];
    }
    if (dep[u] < dep[v]) std::swap(u, v);
    return v;
  };

  int Dist(int u, int v) {
    assert(0 <= u && u < n && 0 <= v && v < n);
    return dep[u] + dep[v] - 2 * dep[Lca(u, v)];
  }

  int Jump(int u, int k) {
    assert(0 <= u && u < n && 0 <= k && k <= dep[u]);
    int d = dep[u] - k;
    while (dep[up[u]] > d) u = fa[up[u]];
    return which[pos[u] - dep[u] + d];
  }

  // u is the root
  int RootedJump(int u, int v, int k) {
    int d = Dist(u, v);
    assert(k <= d);
    int lca = Lca(u, v);
    int d1 = dep[v] - dep[lca];
    if (k <= d1) return Jump(v, k);
    return Jump(u, d - k);
  }

  bool IsAncester(int u, int v) {
    assert(0 <= u && u < n && 0 <= v && v < n);
    return pos[u] <= pos[v] && pos[v] <= pos[u] + size[u] - 1;
  }

  // u is the root
  int RootedFather(int u, int v) {
    assert(0 <= u && u < n && 0 <= v && v < n);
    if (u == v) return -1;
    if (!IsAncester(v, u)) return fa[v];
    int d = dep[u] - dep[v] - 1;
    return Jump(u, d);
  }

  // u is the root
  int RootedSize(int u, int v) {
    assert(0 <= u && u < n && 0 <= v && v < n);
    if (u == v) return n;
    if (!IsAncester(v, u)) return size[v];
    return n - size[RootedFather(u, v)];
  }

  int RootedLca(int a, int b, int c) {
    assert(0 <= a && a < n && 0 <= b && b < n && 0 <= c && c < n);
    return Lca(a, b) ^ Lca(b, c) ^ Lca(c, a);
  }
};

#endif  // HEAVY_LIGHT_DECOMPOSITION_H_