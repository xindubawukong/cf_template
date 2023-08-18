#ifndef TDC_H_
#define TDC_H_

#include <functional>
#include <vector>

template <typename Graph>
struct CentroidDecomposition {
  int n;
  Graph& graph;
  std::vector<int> dep, boss;
  CentroidDecomposition(Graph& graph_) : graph(graph_), n(graph_.n) {
    assert(n > 0);
    assert(!graph.IsDirected());
    assert(graph.edges.size() == n - 1);
    dep.resize(n, 1e9);
    boss.resize(n, -1);
    Build(0, -1);
  }
  int GetCore(int u, int dep0) {
    static std::vector<int> size;
    size.resize(n);
    std::function<void(int, int)> Dfs = [&](int u, int p) {
      size[u] = 1;
      for (int v : graph.Neighbors(u)) {
        if (v == p || dep[v] <= dep0) continue;
        Dfs(v, u);
        size[u] += size[v];
      }
    };
    Dfs(u, -1);
    for (;;) {
      bool flag = false;
      for (int v : graph.Neighbors(u)) {
        if (dep[v] <= dep0) continue;
        if (size[v] * 2 > size[u]) {
          size[u] -= size[v];
          size[v] += size[u];
          u = v;
          flag = true;
          break;
        }
      }
      if (!flag) return u;
    }
  }
  int Build(int u, int b) {
    int dep0 = b == -1 ? 0 : dep[b];
    u = GetCore(u, dep0);
    dep[u] = dep0 + 1;
    for (int v : graph.Neighbors(u)) {
      if (dep[v] <= dep0) continue;
      Build(v, u);
    }
    boss[u] = b;
    return u;
  }
};

#endif  // TDC_H_
