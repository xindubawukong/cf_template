#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <algorithm>
#include <cassert>
#include <limits>
#include <queue>
#include <tuple>
#include <vector>

template <typename Graph, typename F>
auto Dijkstra(const Graph& g, int s, F f) {
  assert(g.IsDirected());
  using dist_t = std::invoke_result_t<F, typename Graph::edge_t>;
  std::vector<dist_t> dist(g.n, std::numeric_limits<dist_t>::max());
  std::vector<int> from(g.n, -1);
  std::vector<bool> visit(g.n);
  dist[s] = 0;
  auto Cmp = [&](auto& p, auto& q) { return p.second > q.second; };
  std::priority_queue<std::pair<int, dist_t>,
                      std::vector<std::pair<int, dist_t>>, decltype(Cmp)>
      q(Cmp);
  q.push({s, 0});
  while (!q.empty()) {
    auto [u, du] = q.top();
    q.pop();
    if (visit[u]) continue;
    visit[u] = true;
    for (auto eid : g.go[u]) {
      auto& e = g.edges[eid];
      int v = e.v;
      if (visit[v]) continue;
      dist_t d = dist[u] + f(e);
      if (d < dist[v]) {
        dist[v] = d;
        from[v] = u;
        q.push({v, d});
      }
    }
  }
  return std::make_tuple(dist, from, visit);
}

#endif  // DIJKSTRA_H_