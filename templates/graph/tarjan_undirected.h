#ifndef TARJAN_UNDIRECTED_H_
#define TARJAN_UNDIRECTED_H_

#include <cassert>
#include <functional>
#include <vector>

template <typename Graph>
struct TarjanUndirected {
  int n, ts;
  std::vector<int> dfn, low, sta;
  std::vector<std::vector<int>> bcc;
  std::vector<bool> cutv;
  TarjanUndirected(Graph& g) {
    assert(!g.IsDirected());
    n = g.n;
    ts = 0;
    dfn.assign(n, -1);
    low.resize(n);
    cutv.resize(n);

    std::function<void(int)> Dfs = [&](int u) {
      dfn[u] = low[u] = ts++;
      sta.push_back(u);
      for (int v : g.Neighbors(u)) {
        if (dfn[v] == -1) {
          Dfs(v);
          low[u] = std::min(low[u], low[v]);
          if (low[v] == dfn[u]) {
            bcc.push_back({});
            for (;;) {
              int t = sta.back();
              sta.pop_back();
              bcc.back().push_back(t);
              if (t == v) break;
            }
            bcc.back().push_back(u);
          }
        } else {
          low[u] = std::min(low[u], dfn[v]);
        }
      }
    };

    for (int i = 0; i < n; i++) {
      if (dfn[i] == -1) {
        sta.clear();
        Dfs(i);
        if (dfn[i] == ts - 1) {
          bcc.push_back({i});
        }
      }
    }

    std::vector<bool> vt(n);
    for (auto& b : bcc) {
      for (int x : b) {
        if (!vt[x]) vt[x] = true;
        else cutv[x] = true;
      }
    }
  }
};

#endif  // TARJAN_UNDIRECTED_H_
