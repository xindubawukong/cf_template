#include "ds/bst_utils.h"

#include <algorithm>
#include <random>
#include <vector>

#include "ds/treap.h"
#include "gtest/gtest.h"

std::mt19937 rng(
    std::chrono::high_resolution_clock::now().time_since_epoch().count());

struct Info {
  Treap<Info>::Node* Node() {
    return reinterpret_cast<Treap<Info>::Node*>(this);
  }
  int val, size;
  Info() : val(0), size(0) {}
  bool NeedPushDown() { return false; }
  void PushDown() {}
  void Update() {
    size = 1;
    auto lch = Node()->lch, rch = Node()->rch;
    if (lch) size += lch->size;
    if (rch) size += rch->size;
  }
};

TEST(BSTUtilsTest, BasicTest) {
  int n = 1000;
  std::vector<int> a(n);
  for (int i = 0; i < n; i++) a[i] = i;

  Treap<Info> treap;
  bst::BuildTree(treap, 0, n - 1, [&](int i, Info* info) { info->val = a[i]; });

  auto CmpVal = [](int val) {
    return [=](Info* info) { return val - info->val; };
  };

  for (int i = 0; i < n; i++) {
    int t = i;
    auto x = bst::Search(treap, CmpVal(t));
    EXPECT_EQ(t, x->val);
    auto [dir, path] = bst::SearchPath(treap, CmpVal(t));
    EXPECT_EQ(0, dir);
    EXPECT_FALSE(path.empty());
    EXPECT_EQ(treap.root, path[0]);
    EXPECT_EQ(x, path.back());
    for (int i = 0; i < (int)path.size() - 1; i++) {
      auto x = path[i], y = path[i + 1];
      EXPECT_TRUE(y == x->lch || y == x->rch);
    }
  }

  for (int i = 0; i < n; i++) {
    int rank = bst::GetRank(treap, CmpVal(i));
    EXPECT_EQ(i + 1, rank);
    auto x = bst::SearchKth(treap, i + 1);
    EXPECT_EQ(i, x->val);
  }

  std::vector<int> b;
  bst::Tranverse(treap, [&](Info* info) { b.push_back(info->val); });
  EXPECT_EQ(a, b);
}