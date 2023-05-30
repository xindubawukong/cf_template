#include "dijkstra.h"

#include <array>
#include <vector>

#include "graph.h"
#include "gtest/gtest.h"

struct Edge {
  int u, v, w;
};

TEST(DijkstraTest, BasicTest) {
  int n = 4, m = 6, s = 0;
  std::vector<std::array<int, 3>> edges = {{0, 1, 2}, {1, 2, 2}, {1, 3, 1},
                                           {0, 2, 5}, {2, 3, 3}, {0, 3, 4}};
  DirectedGraph<Edge> g(n);
  for (auto [u, v, w] : edges) {
    g.AddEdge(Edge({u, v, w}));
  }
  auto [dist, from, visit] = Dijkstra(g, s, [](const Edge &e) { return e.w; });
  EXPECT_EQ(std::vector<int>({0, 2, 4, 3}), dist);
}
