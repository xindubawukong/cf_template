
template <typename Graph>
struct TDC {
  int n;
  const Graph& g;
  vector<int> dep, size, fa, boss, sum;

  TDC(const Graph& g_) : g(g_), n(g_.n) {
    dep.resize(n, 1e9);
    size.resize(n);
    fa.resize(n);
    boss.resize(n);
    sum.resize(n);
    assert(g_.edges.size() == n - 1);
  }

  void Calcsize(int u) {
    size[u] = 1;
    for (int v : g.Neighbors(u)) {
      if (v == fa[u] || dep[v] < dep[u]) continue;
      fa[v] = u;
      Calcsize(v);
      size[u] += size[v];
    }
  }

  int GetCore(int u) {
    fa[u] = -1;
    Calcsize(u);
    for (;;) {
      bool flag = false;
      for (int v : g.Neighbors(u)) {
        if (dep[v] < dep[u]) continue;
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

  void Work(int u, auto& query, auto& ans) {
    // recurse
    for (int v : g.Neighbors(u)) {
      if (dep[v] < dep[u]) continue;
      v = GetCore(v);
      dep[v] = dep[u] + 1;
      boss[v] = u;
      Work(v, query, ans);
    }
  }
};
