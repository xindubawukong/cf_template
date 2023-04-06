#include "graph.h"

#include "gtest/gtest.h"

struct Edge {
  int u, v, cap;
  bool operator==(const Edge& other) const {
    return u == other.u && v == other.v && cap == other.cap;
  }
};

TEST(GraphTest, DirectedGraphTest) {
  DirectedGraph<Edge> g(3);
  g.AddEdge({0, 1});
  g.AddEdge({0, 2});
  std::vector<int> a = {1, 2};
  std::vector<int> tmp;
  for (int x : g.Neighbors(0)) tmp.push_back(x);
  EXPECT_EQ(tmp, a);
  for (int x : g.Neighbors(1)) FAIL();
  for (int x : g.Neighbors(2)) FAIL();

  std::vector<Edge> aa = {{0, 1}, {0, 2}};
  std::vector<Edge> tt;
  for (auto& e : g.Edges(0)) tt.push_back(e);
  EXPECT_EQ(aa, tt);
  for (auto& e : g.Edges(1)) FAIL();
  for (auto& e : g.Edges(2)) FAIL();
}

TEST(GraphTest, UndirectedGraphTest) {
  UndirectedGraph<Edge> g(3);
  g.AddEdge({0, 1});
  g.AddEdge({0, 2});
  std::vector<int> a = {1, 2};
  std::vector<int> b = {0};
  std::vector<int> tmp;
  for (int x : g.Neighbors(0)) tmp.push_back(x);
  EXPECT_EQ(tmp, a);
  tmp.clear();
  for (int x : g.Neighbors(1)) tmp.push_back(x);
  EXPECT_EQ(tmp, b);
  tmp.clear();
  for (int x : g.Neighbors(2)) tmp.push_back(x);
  EXPECT_EQ(tmp, b);

  std::vector<Edge> tt;
  for (auto& e : g.Edges(0)) tt.push_back(e);
  EXPECT_EQ(tt, std::vector<Edge>({{0, 1}, {0, 2}}));
  tt.clear();
  for (auto& e : g.Edges(1)) tt.push_back(e);
  EXPECT_EQ(tt, std::vector<Edge>({{0, 1}}));
  tt.clear();
  for (auto& e : g.Edges(2)) tt.push_back(e);
  EXPECT_EQ(tt, std::vector<Edge>({{0, 2}}));
}
