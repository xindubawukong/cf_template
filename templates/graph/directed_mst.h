#ifndef DIRECTED_MST_H_
#define DIRECTED_MST_H_

// #include <queue>
// #include <vector>

// template <typename Graph, typename F>
// auto DirectedMST(const Graph& g, int s, F f) {
//   assert(g.IsDirected());
//   using dist_t = std::invoke_result_t<F, typename Graph::edge_t>;
//   std::vector<dist_t> dist(g.n, std::numeric_limits<dist_t>::max());
//   std::vector<int> from(g.n, -1);
//   std::vector<bool> visit(g.n);
//   dist[s] = 0;
//   auto Cmp = [&](auto& p, auto& q) { return p.second > q.second; };
//   std::priority_queue<std::pair<int, dist_t>,
//                       std::vector<std::pair<int, dist_t>>, decltype(Cmp)>
//       q(Cmp);
//   q.push({s, 0});
//   dist_t res = 0;
//   while (!q.empty()) {
//     auto [u, du] = q.top();
//     q.pop();
//     if (visit[u]) continue;
//     visit[u] = true;
//     res += du;
//     for (auto eid : g.go[u]) {
//       auto& e = g.edges[eid];
//       int v = e.v;
//       if (visit[v]) continue;
//       dist_t d = f(e);
//       if (d < dist[v]) {
//         dist[v] = d;
//         from[v] = u;
//         q.push({v, d});
//       }
//     }
//   }
//   return std::make_tuple(res, dist, from, visit);
// }

#endif  // DIRECTED_MST_H_
