#include "segment_tree.h"

#include <random>

#include "gtest/gtest.h"

struct Info {
  unsigned int sum, tag;
  SegmentTree<Info>::Node* node;
  Info() : sum(0), tag(0) {}
  bool NeedPushDown() { return tag > 0; }
  void Plus(unsigned int x) {
    tag += x;
    sum += (node->r - node->l + 1) * x;
  }
  void PushDown() {
    assert(tag > 0);
    if (node->lch) {
      node->lch->info.Plus(tag);
    }
    if (node->rch) {
      node->rch->info.Plus(tag);
    }
    tag = 0;
  }
  void Update() {
    sum = 0;
    if (node->lch) {
      sum += node->lch->info.sum;
    }
    if (node->rch) {
      sum += node->rch->info.sum;
    }
  }
};

TEST(SegmentTreeTest, BasicTest) {
  std::mt19937 rng(
      std::chrono::high_resolution_clock::now().time_since_epoch().count());
  int n = rng() % 1000 + 500;
  std::vector<unsigned int> a(n);
  for (int i = 0; i < n; i++) a[i] = rng();
  unsigned int sum = std::accumulate(a.begin(), a.end(), 0u);
  SegmentTree<Info> tree(0, n - 1, false);
  tree.root =
      tree.BuildTree(0, n - 1, [&](int i, Info& info) { info.sum = a[i]; });
  EXPECT_EQ(sum, tree.root->info.sum);

  int q = rng() % 1000 + 500;
  while (q--) {
    int l = rng() % n, r = rng() % n;
    if (l > r) std::swap(l, r);
    unsigned int x = rng();
    for (int i = l; i <= r; i++) a[i] += x;
    tree.Modify(l, r, [&](Info& info) { info.Plus(x); });

    l = rng() % n, r = rng() % n;
    if (l > r) std::swap(l, r);
    x = 0;
    for (int i = l; i <= r; i++) x += a[i];
    unsigned int y = 0;
    for (auto node : tree.GetAllNodes(l, r)) {
      y += node->info.sum;
    }
    EXPECT_EQ(x, y);
  }
}