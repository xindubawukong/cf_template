#ifndef TARJAN_DIRECTED_H_
#define TARJAN_DIRECTED_H_

#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>

template <typename Graph>
struct TarjanDirected {
  std::vector<int> dfn, low, belong, sta;
  std::vector<bool> vt;
  int ts, scc_cnt;
  TarjanDirected(Graph& g) {
    assert(g.IsDirected());
    dfn.resize(g.n);
    low.resize(g.n);
    belong.resize(g.n);
    std::fill(belong.begin(), belong.end(), -1);
    vt.resize(g.n);
    ts = 0;
    scc_cnt = 0;
    std::function<void(int)> Dfs = [&](int u) {
      dfn[u] = low[u] = ++ts;
      vt[u] = true;
      sta.push_back(u);
      for (int eid : g.go[u]) {
        auto& e = g.edges[eid];
        if (!vt[e.v]) {
          Dfs(e.v);
          low[u] = std::min(low[u], low[e.v]);
        } else if (belong[e.v] == -1) {
          low[u] = std::min(low[u], dfn[e.v]);
        }
      }
      if (low[u] == dfn[u]) {
        while (!sta.empty()) {
          int v = sta.back();
          belong[v] = scc_cnt;
          sta.pop_back();
          if (v == u) break;
        }
        scc_cnt++;
      }
    };
    for (int i = 0; i < g.n; i++) {
      if (!vt[i]) Dfs(i);
    }
  }
};

#endif  // TARJAN_DIRECTED_H_
