#ifndef HEAVY_LIGHT_DECOMPOSITION_H_
#define HEAVY_LIGHT_DECOMPOSITION_H_

#include <vector>

#include "segment_tree.h"

template <typename Info>
struct HeavyLightDecomposition {
  int n, root;
  std::vector<std::vector<int>> go;
  std::vector<int> size, dep, pos, which, heavy, fa, up;
  SegmentTree<Info> segtree;

  HeavyLightDecomposition(int n_, int root_)
      : n(n_), root(root_), segtree(SegmentTree<Info>(0, n - 1)) {
    go.resize(n);
    size.resize(n);
    dep.resize(n);
    pos.resize(n, -1);
    which.resize(n, -1);
    heavy.resize(n, -1);
    fa.resize(n, -1);
    up.resize(n, -1);
  }

  void AddEdge(int u, int v) {
    assert(0 <= u && u < n && 0 <= v && v < n);
    go[u].push_back(v);
    go[v].push_back(u);
  }

  template <typename F>
  void Build(F f) {
    std::function<void(int)> Dfs1 = [&](int u) {
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
    Dfs1(root);
    up[root] = root;
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
      assert(cnt - pos[u] == size[u]);
    };
    Dfs2(root);
    assert(cnt == n);
    segtree.root = segtree.BuildTree(
        0, n - 1, [&](int i, Info& info) { f(which[i], info); });
  }

  template <typename F>
  void ModifyOnPath(int u, int v, F f) {
    while (up[u] != up[v]) {
      if (dep[up[u]] < dep[up[v]]) std::swap(u, v);
      segtree.Modify(pos[up[u]], pos[u], f);
      u = fa[up[u]];
    }
    if (dep[u] < dep[v]) std::swap(u, v);
    segtree.Modify(pos[v], pos[u], f);
  }

  template <typename F>
  void ModifyInSubTree(int u, F f) {
    segtree.Modify(pos[u], pos[u] + size[u] - 1, f);
  }

  std::vector<Info> GetAllOnPath(int u, int v) {
    std::vector<Info> res;
    while (up[u] != up[v]) {
      if (dep[up[u]] < dep[up[v]]) std::swap(u, v);
      auto nodes = segtree.GetAllNodes(pos[up[u]], pos[u]);
      for (auto&& node : nodes) {
        res.push_back(node->info);
      }
      u = fa[up[u]];
    }
    if (dep[u] < dep[v]) std::swap(u, v);
    auto nodes = segtree.GetAllNodes(pos[v], pos[u]);
    for (auto&& node : nodes) {
      res.push_back(node->info);
    }
    return res;
  }

  std::vector<Info> GetAllInSubTree(int u) {
    std::vector<Info> res;
    auto nodes = segtree.GetAllNodes(pos[u], pos[u] + size[u] - 1);
    for (auto&& node : nodes) {
      res.push_back(node->info);
    }
    return res;
  }
};

#endif  // HEAVY_LIGHT_DECOMPOSITION_H_