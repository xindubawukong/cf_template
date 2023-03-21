#include "flow.h"

#include <array>

#include "graph.h"
#include "gtest/gtest.h"

struct Edge {
  int u, v, cap;
};

TEST(FlowTest, MaxFlowTest) {
  int n = 4, m = 5, s = 3, t = 2;
  std::vector<std::array<int, 3>> edges = {
      {3, 1, 30}, {3, 2, 20}, {1, 2, 20}, {1, 0, 30}, {0, 2, 30}};
  FlowGraph<Edge> graph(n);
  for (auto [u, v, cap] : edges) {
    graph.AddFlowEdge(Edge{u, v, cap});
  }
  MaxFlow mf(graph);
  EXPECT_EQ(50, mf.Solve(s, t, 1000));
}