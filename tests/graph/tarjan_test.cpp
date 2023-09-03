#include "graph/tarjan_directed.h"

#include "graph/graph.h"
#include "gtest/gtest.h"

TEST(TarjanTest, BasicTest) {
  DirectedGraph<EdgeBase> g(3);
  g.AddEdge({0, 1});
  g.AddEdge({1, 0});
  g.AddEdge({1, 2});
  TarjanDirected tj(g);
  EXPECT_EQ(tj.belong[0], tj.belong[1]);
  EXPECT_NE(tj.belong[0], tj.belong[2]);
}
