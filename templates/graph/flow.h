#ifndef FLOW_H_
#define FLOW_H_

#include <cassert>
#include <functional>
#include <limits>
#include <queue>
#include <vector>

template <typename T = int>
struct Matching {
  int n, m, res = 0, ts = 0;
  std::vector<std::vector<int>> go;
  std::vector<int> vt, pa, pb;
  Matching(int n_, int m_) : n(n_), m(m_) {
    assert(n > 0 && m > 0);
    go.resize(n);
    vt.resize(n, -1);
    pa.resize(n, -1);
    pb.resize(m, -1);
  }
  void AddEdge(int u, int v) {
    assert(u >= 0 && u < n && v >= 0 && v < m);
    go[u].push_back(v);
  }
  bool Dfs(int u) {
    vt[u] = ts;
    for (int v : go[u]) {
      if (pb[v] == -1) {
        pa[u] = v;
        pb[v] = u;
        return true;
      }
    }
    for (int v : go[u]) {
      if (vt[pb[v]] != ts && Dfs(pb[v])) {
        pa[u] = v;
        pb[v] = u;
        return true;
      }
    }
    return false;
  }
  int Build() {
    for (int i = 0; i < n; i++) {
      if (pa[i] == -1) {
        ts++;
        if (Dfs(i)) res++;
      }
    }
    return res;
  }
};

template <typename FlowGraph>
struct MaxFlow {
  using flow_t = typename FlowGraph::flow_t;
  FlowGraph& graph;
  MaxFlow(FlowGraph& graph_) : graph(graph_) { assert(graph.IsDirected()); }

  flow_t Solve(int s, int t, flow_t new_flow) {
    assert(0 <= s && s < graph.n && 0 <= t && t < graph.n);
    std::vector<int> dep(graph.n);
    std::vector<int> cur(graph.n);
    auto& go = graph.go;
    auto& edges = graph.edges;
    auto Bfs = [&]() -> bool {
      std::fill(dep.begin(), dep.end(), -1);
      std::fill(cur.begin(), cur.end(), 0);
      dep[s] = 0;
      std::vector<int> q;
      q.push_back(s);
      for (int i = 0; i < q.size(); i++) {
        int u = q[i];
        for (auto eid : go[u]) {
          auto& e = edges[eid];
          if (e.cap > graph.eps && dep[e.v] == -1) {
            dep[e.v] = dep[u] + 1;
            q.push_back(e.v);
            if (e.v == t) return true;
          }
        }
      }
      return dep[t] != -1;
    };
    std::function<flow_t(int, flow_t)> Dfs = [&](int u, flow_t flow) {
      if (u == t) return flow;
      flow_t res = 0;
      for (int& i = cur[u]; i < go[u].size(); i++) {
        auto eid = go[u][i];
        auto& e = edges[eid];
        auto& back = edges[eid ^ 1];
        if (e.cap <= graph.eps || dep[e.v] != dep[u] + 1) continue;
        flow_t df = Dfs(e.v, std::min(e.cap, flow));
        if (df <= graph.eps) dep[e.v] = -1;
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

template <typename FlowGraph>
struct MinCostMaxFlow {
  using flow_t = typename FlowGraph::flow_t;
  using cost_t = decltype(std::declval<typename FlowGraph::edge_t>().cost);
  FlowGraph& graph;
  MinCostMaxFlow(FlowGraph& graph_) : graph(graph_) {}

  std::pair<flow_t, cost_t> Solve(int s, int t) {
    assert(0 <= s && s < graph.n && 0 <= t && t < graph.n);
    std::vector<int> pre(graph.n);
    auto Spfa = [&]() -> bool {
      std::fill(pre.begin(), pre.end(), -1);
      std::vector<cost_t> dist(graph.n);
      std::vector<bool> inq(graph.n), vt(graph.n);
      dist[s] = 0;
      inq[s] = true;
      vt[s] = true;
      std::queue<int> q;
      q.push(s);
      while (!q.empty()) {
        int u = q.front();
        q.pop();
        inq[u] = false;
        for (auto eid : graph.go[u]) {
          auto& e = graph.edges[eid];
          if (e.cap > graph.eps && (!vt[e.v] || dist[e.v] > dist[u] + e.cost)) {
            pre[e.v] = eid;
            dist[e.v] = dist[u] + e.cost;
            vt[e.v] = true;
            if (!inq[e.v]) {
              inq[e.v] = true;
              q.push(e.v);
            }
          }
        }
      }
      return vt[t];
    };
    auto Calc = [&]() {
      flow_t flow = std::numeric_limits<flow_t>::max();
      for (auto eid = pre[t]; eid != -1;) {
        auto& e = graph.edges[eid];
        flow = std::min(flow, e.cap);
        eid = pre[e.u];
      }
      cost_t cost = 0;
      for (auto eid = pre[t]; eid != -1;) {
        auto& e = graph.edges[eid];
        auto& back = graph.edges[eid ^ 1];
        cost += flow * e.cost;
        e.cap -= flow;
        back.cap += flow;
        eid = pre[e.u];
      }
      return std::make_pair(flow, cost);
    };
    flow_t flow = 0;
    cost_t cost = 0;
    while (Spfa()) {
      auto [f, c] = Calc();
      flow += f, cost += c;
    }
    return {flow, cost};
  }
};

#endif  // FLOW_H_