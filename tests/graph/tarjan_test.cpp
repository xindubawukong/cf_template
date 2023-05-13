#include "tarjan.h"

#include "graph.h"
#include "gtest/gtest.h"

struct Edge {
  int u, v;
};

TEST(TarjanTest, BasicTest) {
  DirectedGraph<Edge> g(3);
  g.AddEdge({0, 1});
  g.AddEdge({1, 0});
  g.AddEdge({1, 2});
  TarjanDirected tj(g);
  EXPECT_EQ(tj.belong[0], tj.belong[1]);
  EXPECT_NE(tj.belong[0], tj.belong[2]);
}
