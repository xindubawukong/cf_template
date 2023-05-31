#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <algorithm>
#include <limits>
#include <queue>
#include <tuple>
#include <vector>

template <typename Graph, typename F>
auto Dijkstra(const Graph &g, int s, F f) {
  static_assert(Graph::is_directed::value);
  using dist_t = std::invoke_result_t<F, typename Graph::edge_t>;
  std::vector<dist_t> dist(g.n, std::numeric_limits<dist_t>::max());
  std::vector<int> from(g.n);
  std::vector<bool> visit(g.n);
  dist[s] = 0;
  auto Cmp = [&](int &u, int &v) { return dist[u] > dist[v]; };
  std::priority_queue<int, std::vector<int>, decltype(Cmp)> q(Cmp);
  q.push(s);
  while (!q.empty()) {
    int u = q.top();
    q.pop();
    if (visit[u]) continue;
    visit[u] = true;
    for (auto eid : g.go[u]) {
      auto &e = g.edges[eid];
      int v = e.v;
      if (visit[v]) continue;
      dist_t d = dist[u] + f(e);
      if (d < dist[v]) {
        dist[v] = d;
        from[v] = u;
        q.push(v);
      }
    }
  }
  return std::make_tuple(dist, from, visit);
}

#endif  // DIJKSTRA_H_