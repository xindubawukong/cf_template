#include "ds/treap.h"

#include <algorithm>
#include <random>
#include <vector>

#include "ds/bst_utils.h"
#include "gtest/gtest.h"

struct Info {
  Treap<Info>::Node* Node() {
    return reinterpret_cast<Treap<Info>::Node*>(this);
  }
  int val, size;
  Info(int val_ = 0) : val(val_), size(1) {}
  bool NeedPushDown() { return false; }
  void PushDown() {}
  void Update() {
    size = 1;
    if (Node()->lch) {
      size += Node()->lch->size;
    }
    if (Node()->rch) {
      size += Node()->rch->size;
    }
  }
};

TEST(TreapTest, BasicTest) {
  std::mt19937 rng(
      std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::vector<int> a;
  for (int i = 0; i < 20000; i++) {
    a.push_back(rng());
  }
  std::sort(a.begin(), a.end());
  a.resize(std::unique(a.begin(), a.end()) - a.begin());
  int n = a.size();

  Treap<Info> treap;
  for (int i = 0; i < n; i++) {
    auto [t1, t2, t3] = treap.Split(treap.root, [&](auto& info) {
      if (a[i] < info->val) return -1;
      if (a[i] == info->val) return 0;
      return 1;
    });
    ASSERT_EQ(t2, nullptr);
    t2 = treap.Create();
    t2->val = a[i];
    treap.root = treap.Join(t1, t2, t3);
  }
  EXPECT_EQ(n, treap.root->size);

  for (int k = 1; k <= n; k++) {
    auto [t1, t2, t3] = treap.SplitKth(treap.root, k);
    EXPECT_EQ(a[k - 1], t2->val);
    treap.root = treap.Join(t1, t2, t3);
  }
}
