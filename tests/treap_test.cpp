#include "treap.h"

#include <random>
#include <vector>

#include "gtest/gtest.h"

struct Info {
  Treap<Info>::Node *Node() {
    return reinterpret_cast<Treap<Info>::Node *>(this);
  }
  int val, size;
  Info(int val_) : val(val_), size(1) {}
  bool NeedPushDown() { return false; }
  void PushDown() {}
  void Update() {
    size = 1;
    if (Node()->lch) {
      size += Node()->lch->info.size;
    }
    if (Node()->rch) {
      size += Node()->rch->info.size;
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
    auto [t1, t2, t3] = treap.Split(treap.root, [&](auto &info) {
      if (a[i] < info.val) return -1;
      if (a[i] == info.val) return 0;
      return 1;
    });
    ASSERT_EQ(t2, nullptr);
    auto x = new Treap<Info>::Node(Info(a[i]));
    treap.root = treap.Join(t1, x, t3);
  }
  EXPECT_EQ(n, treap.root->info.size);
  std::vector<int> b;
  treap.Tranverse(treap.root, [&](auto &info) { b.push_back(info.val); });
  EXPECT_EQ(a, b);

  for (int i = 0; i < n; i++) {
    auto path = treap.Search([&](auto &info) {
      if (a[i] < info.val) return -1;
      if (a[i] == info.val) return 0;
      return 1;
    });
    EXPECT_FALSE(path.empty());
    for (int j = 0; j < path.size() - 1; j++) {
      EXPECT_TRUE(path[j]->lch == path[j + 1] || path[j]->rch == path[j + 1]);
    }
    EXPECT_EQ(a[i], path.back()->info.val);
  }
}
