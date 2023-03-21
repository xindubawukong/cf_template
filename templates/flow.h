#ifndef FLOW_H_
#define FLOW_H_

#include <queue>
#include <vector>

template <typename FlowGraph>
struct MaxFlow {
  using flow_t = typename FlowGraph::flow_t;
  FlowGraph& graph;
  MaxFlow(FlowGraph& graph_) : graph(graph_) {}

  flow_t Solve(int s, int t, flow_t new_flow) {
    assert(0 <= s && s < graph.n && 0 <= t && t < graph.n);
    std::vector<int> dep(graph.n);
    std::vector<int> cur(graph.n);
    auto Bfs = [&]() -> bool {
      std::fill(dep.begin(), dep.end(), -1);
      std::fill(cur.begin(), cur.end(), 0);
      dep[s] = 0;
      std::queue<int> q;
      q.push(s);
      while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto eid : graph.go[u]) {
          auto& e = graph.edges[eid];
          if (e.cap > graph.eps && dep[e.v] == -1) {
            dep[e.v] = dep[u] + 1;
            q.push(e.v);
          }
        }
      }
      return dep[t] != -1;
    };
    std::function<flow_t(int, flow_t)> Dfs = [&](int u, flow_t flow) {
      if (u == t) return flow;
      flow_t res = 0;
      for (int& i = cur[u]; i < graph.go[u].size(); i++) {
        auto eid = graph.go[u][i];
        auto& e = graph.edges[eid];
        auto& back = graph.edges[eid ^ 1];
        if (e.cap <= graph.eps || dep[e.v] != dep[u] + 1) continue;
        flow_t df = Dfs(e.v, std::min(e.cap, flow));
        flow -= df;
        e.cap -= df;
        back.cap += df;
        res += df;
        if (flow <= graph.eps) break;
      }
      if (res <= graph.eps) dep[u] = -1;
      return res;
    };
    flow_t ans = 0;
    while (Bfs() && new_flow - ans > graph.eps) {
      ans += Dfs(s, new_flow - ans);
    }
    return ans;
  }
};

#endif  // FLOW_H_