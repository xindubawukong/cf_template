#ifndef TARJAN_H_
#define TARJAN_H_

#include <algorithm>
#include <functional>
#include <stack>
#include <vector>

template <typename Graph> struct TarjanDirected {
  static_assert(Graph::is_directed::value);
  std::vector<int> dfn, low, belong;
  std::vector<bool> visit;
  std::stack<int> sta;
  int ts, scc_cnt;
  TarjanDirected(Graph &g) {
    dfn.resize(g.n);
    low.resize(g.n);
    belong.resize(g.n);
    std::fill(belong.begin(), belong.end(), -1);
    visit.resize(g.n);
    ts = 0;
    scc_cnt = 0;
    std::function<void(int)> Dfs = [&](int u) {
      dfn[u] = low[u] = ++ts;
      visit[u] = true;
      sta.push(u);
      for (int eid : g.go[u]) {
        auto &e = g.edges[eid];
        if (!visit[e.v]) {
          Dfs(e.v);
          low[u] = std::min(low[u], low[e.v]);
        } else if (belong[e.v] == -1) {
          low[u] = std::min(low[u], dfn[e.v]);
        }
      }
      if (low[u] == dfn[u]) {
        while (!sta.empty()) {
          int v = sta.top();
          belong[v] = scc_cnt;
          sta.pop();
          if (v == u)
            break;
        }
        scc_cnt++;
      }
    };
    for (int i = 0; i < g.n; i++) {
      if (!visit[i]) {
        Dfs(i);
      }
    }
  }
};

#endif // TARJAN_H_
