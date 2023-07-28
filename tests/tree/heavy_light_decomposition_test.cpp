#include "tree/heavy_light_decomposition.h"

#include <random>
#include <vector>

#include "gtest/gtest.h"

std::vector<std::vector<int>> GetTree(int n) {
  std::mt19937 rng(
      std::chrono::high_resolution_clock::now().time_since_epoch().count());
  assert(n > 0);
  std::vector<std::vector<int>> go(n);
  for (int i = 1; i < n; i++) {
    int fa = rng() % i;
    go[i].push_back(fa);
    go[fa].push_back(i);
  }
  for (int i = 0; i < n; i++) {
    std::shuffle(go[i].begin(), go[i].end(), rng);
  }
  return go;
}

auto Bfs(int n, auto &go, int root) {
  std::vector<int> fa(n), size(n), dep(n), que;
  fa[root] = -1;
  dep[root] = 0;
  que.push_back(root);
  for (int i = 0; i < que.size(); i++) {
    int u = que[i];
    for (int v : go[u]) {
      if (v == fa[u]) continue;
      fa[v] = u;
      dep[v] = dep[u] + 1;
      que.push_back(v);
    }
  }
  assert(que.size() == n);
  for (int i = n - 1; i >= 0; i--) {
    int u = que[i];
    size[u] = 1;
    for (int v : go[u]) {
      if (v == fa[u]) continue;
      size[u] += size[v];
    }
  }
  return std::make_tuple(fa, size, dep);
}

int GetLca(int n, auto &fa, int u, int v) {
  std::vector<bool> vt(n, false);
  for (int i = u; i != -1; i = fa[i]) vt[i] = true;
  for (int i = v; i != -1; i = fa[i]) {
    if (vt[i]) return i;
  }
  assert(false);
}

TEST(HeavyLightDecompositionTest, BasicTest) {
  std::mt19937 rng(
      std::chrono::high_resolution_clock::now().time_since_epoch().count());
  int n = rng() % 1000 + 500;
  auto go = GetTree(n);

  HeavyLightDecomposition hld(n);
  hld.go = go;
  for (int i = 0; i < 10; i++) {
    hld.Build(rng() % n);
  }

  auto [fa, size, dep] = Bfs(n, go, hld.root);
  EXPECT_EQ(fa, hld.fa);
  EXPECT_EQ(size, hld.size);
  EXPECT_EQ(dep, hld.dep);
  for (int tc = 0; tc < 1000; tc++) {
    int u = rng() % n;
    auto [l, r] = hld.SubTreeInterval(u);
    EXPECT_EQ(r - l + 1, size[u]);
    for (int i = l; i <= r; i++) {
      int v = hld.which[i];
      EXPECT_TRUE(hld.IsAncester(u, v));
      EXPECT_EQ(u, hld.Jump(v, dep[v] - dep[u]));
    }
  }

  for (int tc = 0; tc < 1000; tc++) {
    int u = rng() % n, v = rng() % n;
    auto intervals = hld.PathIntervals(u, v);
    EXPECT_LT(intervals.size(), 30);
    std::set<int> s1;
    for (auto [l, r] : intervals) {
      for (int i = l; i <= r; i++) {
        EXPECT_FALSE(s1.contains(hld.which[i]));
        s1.insert(hld.which[i]);
      }
    }
    auto [fa, size, dep] = Bfs(n, go, u);
    std::set<int> s2;
    for (int i = v; i != -1; i = fa[i]) {
      s2.insert(i);
    }
    EXPECT_EQ(s1, s2);
  }
}

TEST(HeavyLightDecompositionTest, LcaTest) {
  std::mt19937 rng(
      std::chrono::high_resolution_clock::now().time_since_epoch().count());
  int n = rng() % 1000 + 500;
  auto go = GetTree(n);

  HeavyLightDecomposition hld(n);
  hld.go = go;
  for (int i = 0; i < 10; i++) {
    hld.Build(rng() % n);
  }

  auto [fa, size, dep] = Bfs(n, go, hld.root);
  for (int tc = 0; tc < 1000; tc++) {
    int u = rng() % n, v = rng() % n;
    int lca = hld.Lca(u, v);
    EXPECT_EQ(lca, hld.Jump(u, dep[u] - dep[lca]));
    EXPECT_EQ(lca, hld.Jump(v, dep[v] - dep[lca]));
    EXPECT_EQ(hld.Dist(u, v), dep[u] + dep[v] - dep[lca] * 2);
    EXPECT_TRUE(hld.IsAncester(lca, u));
    EXPECT_TRUE(hld.IsAncester(lca, v));
    EXPECT_EQ(GetLca(n, fa, u, v), lca);
  }
}

TEST(HeavyLightDecompositionTest, RootedTest) {
  std::mt19937 rng(
      std::chrono::high_resolution_clock::now().time_since_epoch().count());
  int n = rng() % 1000 + 500;
  auto go = GetTree(n);

  HeavyLightDecomposition hld(n);
  hld.go = go;
  for (int i = 0; i < 10; i++) {
    hld.Build(rng() % n);
  }

  for (int tc = 0; tc < 1000; tc++) {
    int u = rng() % n, v = rng() % n, w = rng() % n;
    EXPECT_EQ(-1, hld.RootedFather(u, u));
    auto [fa, size, dep] = Bfs(n, go, u);
    EXPECT_EQ(fa[v], hld.RootedFather(u, v));
    EXPECT_EQ(size[v], hld.RootedSize(u, v));
    EXPECT_EQ(GetLca(n, fa, v, w), hld.RootedLca(u, v, w));
  }
}
