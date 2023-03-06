#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <queue>
#include <vector>

template <typename Graph, typename F>
auto Dijkstra(Graph& g, int s, F f) {
  static_assert(Graph::is_directed::value);
  using dist_t = std::invoke_result_t<F, typename Graph::edge_t>;
  std::vector<dist_t> dist(g.n, std::numeric_limits<dist_t>::max());
  std::vector<int> from(g.n);
  std::vector<bool> visit(g.n);
  dist[s] = 0;
  auto Cmp = [](auto& p, auto& q) { return p.first > q.first; };
  std::priority_queue<std::pair<dist_t, int>,
                      std::vector<std::pair<dist_t, int>>, decltype(Cmp)>
      q;
  q.push({0, s});
  while (!q.empty()) {
    auto [du, u] = q.top();
    q.pop();
    if (visit[u]) continue;
    visit[u] = true;
    for (auto eid : g.go[u]) {
      auto& e = g.edges[eid];
      int v = e.v;
      if (visit[v]) continue;
      dist_t d = du + f(e);
      if (d < dist[v]) {
        dist[v] = d;
        from[v] = u;
        q.push({d, v});
      }
    }
  }
  return std::make_tuple(dist, from, visit);
}

#endif  // DIJKSTRA_H_