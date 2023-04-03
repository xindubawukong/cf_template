#ifndef HEAVY_LIGHT_DECOMPOSITION_H_
#define HEAVY_LIGHT_DECOMPOSITION_H_

#include <vector>

#include "segment_tree.h"

struct HeavyLightDecomposition {
  int n, root;
  std::vector<std::vector<int>> go;
  std::vector<int> size, dep, pos, which, heavy, fa, up;

  HeavyLightDecomposition(int n_) : n(n_) { assert(n > 0); }

  void AddEdge(int u, int v) {
    assert(0 <= u && u < n && 0 <= v && v < n);
    go[u].push_back(v);
    go[v].push_back(u);
  }

  void Build(int root_) {
    go.resize(n);
    size.resize(n);
    dep.resize(n);
    pos.resize(n);
    which.resize(n);
    heavy.resize(n);
    fa.resize(n);
    up.resize(n);
    root = root_;
    std::function<void(int)> Dfs1 = [&](int u) {
      heavy[u] = -1;
      size[u] = 1;
      for (int v : go[u]) {
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
      for (int v : go[u]) {
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

  std::vector<std::pair<int, int>> GetPathIntervals(int u, int v) {
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

  std::pair<int, int> GetSubTreeInterval(int u) {
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
    assert(0 <= u && u < n && k >= 0);
    if (dep[u] < k) return -1;
    int d = dep[u] - k;
    while (dep[up[u]] > d) u = fa[up[u]];
    return which[pos[u] - dep[u] + d];
  }

  bool IsAncester(int u, int v) {
    assert(0 <= u && u < n && 0 <= v && v < n);
    return pos[u] <= pos[v] && pos[v] <= pos[u] + size[u] - 1;
  }

  int RootedParent(int root, int u) {
    assert(0 <= root && root < n && 0 <= u && u < n);
    if (root == u) return u;
    if (!IsAncester(u, root)) return fa[u];
    int d = dep[root] - dep[u] - 1;
    return Jump(root, d);
  }

  int RootedSize(int root, int u) {
    assert(0 <= root && root < n && 0 <= u && u < n);
    if (root == u) return n;
    if (!IsAncester(u, root)) {
      return size[u];
    }
    return n - size[RootedParent(root, u)];
  }

  int RootedLca(int a, int b, int c) {
    assert(0 <= a && a < n && 0 <= b && b < n && 0 <= c && c < n);
    return Lca(a, b) ^ Lca(b, c) ^ Lca(c, a);
  }
};

#endif  // HEAVY_LIGHT_DECOMPOSITION_H_