#include "graph/flow.h"

#include <array>
#include <vector>

#include "graph/graph.h"
#include "graph/matching.h"
#include "gtest/gtest.h"

TEST(FlowTest, MatchingTest) {
  int n = 3, m = 3;
  Matching matching(n, m);
  matching.AddEdge(0, 0);
  matching.AddEdge(0, 1);
  matching.AddEdge(1, 0);
  matching.AddEdge(2, 2);
  int res = matching.Solve();
  EXPECT_EQ(3, res);
  EXPECT_EQ(3, matching.res);
  EXPECT_EQ(std::vector<int>({1, 0, 2}), matching.pa);
  EXPECT_EQ(std::vector<int>({1, 0, 2}), matching.pb);
}

struct Edge {
  int u, v, cap, cost;
};

TEST(FlowTest, MaxFlowTest) {
  int n = 4, s = 3, t = 2;
  std::vector<std::array<int, 3>> edges = {
      {3, 1, 30}, {3, 2, 20}, {1, 2, 20}, {1, 0, 30}, {0, 2, 30}};
  FlowGraph<Edge> graph(n);
  for (auto [u, v, cap] : edges) {
    graph.AddFlowEdge(Edge{u, v, cap, 0});
  }
  MaxFlow mf(graph);
  EXPECT_EQ(50, mf.Solve(s, t, 1000));
}

TEST(FlowTest, MinCostMaxFlowTest) {
  int n = 4, s = 3, t = 2;
  std::vector<std::array<int, 4>> edges = {{3, 1, 30, 2},
                                           {3, 2, 20, 3},
                                           {1, 2, 20, 1},
                                           {1, 0, 30, 9},
                                           {0, 2, 30, 5}};
  FlowGraph<Edge> graph(n);
  for (auto [u, v, cap, cost] : edges) {
    graph.AddFlowEdgeWithCost(Edge{u, v, cap, cost});
  }
  MinCostMaxFlow mf(graph);
  auto [flow, cost] = mf.Solve(s, t);
  EXPECT_EQ(50, flow);
  EXPECT_EQ(280, cost);
}
