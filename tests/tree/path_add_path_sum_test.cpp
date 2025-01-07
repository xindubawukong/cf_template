
#include <random>

#include "ds/segment_tree.h"
#include "graph/graph.h"
#include "gtest/gtest.h"
#include "tree/heavy_light_decomposition.h"
#include "tree/link_cut_tree.h"

struct SegInfo {
  int64_t sum, tag;
  auto Node() { return reinterpret_cast<SegmentTree<SegInfo>::Node*>(this); }
  SegInfo() : sum(0), tag(0) {}
  bool NeedPushDown() { return tag != 0; }
  void Add(int64_t x) {
    int cnt = Node()->r - Node()->l + 1;
    sum += x * cnt;
    tag += x;
  }
  void PushDown() {
    auto lch = Node()->lch, rch = Node()->rch;
    if (lch) lch->Add(tag);
    if (rch) rch->Add(tag);
    tag = 0;
  }
  void Update() {
    auto lch = Node()->lch, rch = Node()->rch;
    sum = tag;
    if (lch) sum += lch->sum;
    if (rch) sum += rch->sum;
  }
};

struct LctInfo {
  int cnt;
  int64_t val, sum, tag;
  LctInfo() : cnt(0), val(0), sum(0), tag(0) {}
  void Reverse() {}
  bool NeedPushDown() { return tag != 0; }
  void Add(int64_t x) {
    val += x;
    sum += x * cnt;
    tag += x;
  }
  void PushDown(LctInfo* a, LctInfo* b) {
    if (a) a->Add(tag);
    if (b) b->Add(tag);
    tag = 0;
  }
  void Update(LctInfo* a, LctInfo* b) {
    cnt = 1;
    sum = val;
    if (a) cnt += a->cnt, sum += a->sum;
    if (b) cnt += b->cnt, sum += b->sum;
  }
};

TEST(PathAddPathSumTest, BasicTest) {
  std::mt19937 rng(
      std::chrono::high_resolution_clock::now().time_since_epoch().count());
  int n = 100000, q = 100000, M = 1e5;
  UndirectedGraph<EdgeBase> g(n);
  for (int i = 1; i < n; i++) {
    int j;
    if (i < 10) {
      j = rng() % i;
    } else {
      j = rng() % 10 + i - 10;
    }
    g.AddEdge({i, j});
  }

  HeavyLightDecomposition hld(g, 0);
  SegmentTree<SegInfo> seg(0, n - 1);
  seg.root = seg.BuildTree(0, n - 1, [&](int i, SegInfo* info) {});

  LinkCutTree<LctInfo> lct(n);
  for (auto [u, v] : g.edges) lct.Link(u, v);

  for (int i = 0; i < q; i++) {
    int op = rng() % 2, u = rng() % n, v = rng() % n;
    if (op == 0) {
      int64_t x = rng() % M;
      for (auto [l, r] : hld.PathIntervals(u, v)) {
        seg.Modify(l, r, [&](SegInfo* info) { info->Add(x); });
      }
      lct.MakeRoot(u);
      lct.Access(v);
      lct.Splay(v);
      lct.node[v]->Add(x);
    } else {
      int64_t ans1 = 0;
      for (auto [l, r] : hld.PathIntervals(u, v)) {
        for (auto node : seg.GetAllNodes(l, r)) ans1 += node->sum;
      }
      lct.MakeRoot(u);
      lct.Access(v);
      lct.Splay(v);
      int64_t ans2 = lct.node[v]->sum;
      EXPECT_EQ(ans1, ans2);
    }
  }
}
