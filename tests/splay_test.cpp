#include "splay.h"

#include <algorithm>
#include <random>
#include <vector>

#include "bst_utils.h"
#include "gtest/gtest.h"

std::mt19937 rng(
    std::chrono::high_resolution_clock::now().time_since_epoch().count());

struct Info {
  SplayTree<Info>::Node* Node() {
    return reinterpret_cast<SplayTree<Info>::Node*>(this);
  }
  int val;
  Info() {}
  bool NeedPushDown() { return false; }
  void PushDown() {}
  void Update() {}
};

TEST(SplayTest, BasicTest) {
  int n = 1000;
  std::vector<int> a(n);
  for (int i = 0; i < n; i++) a[i] = i;
  std::shuffle(a.begin(), a.end(), rng);
  SplayTree<Info> splay;
  for (int val : a) {
    Info info;
    info.val = val;
    auto x = new SplayTree<Info>::Node(info);
    auto [dir, path] =
        bst::SearchPath(splay, [&](Info* info) { return val - info->val; });
    EXPECT_NE(0, dir);
    EXPECT_TRUE(dir == -1 || dir == 1);
    if (path.empty()) {
      splay.root = x;
    } else {
      if (dir == -1) {
        path.back()->lch = x;
      } else {
        path.back()->rch = x;
      }
      x->fa = path.back();
    }
    splay.Splay(x, nullptr);
    EXPECT_EQ(splay.root, x);
  }

  std::vector<int> b;
  bst::Tranverse(splay, [&](Info* info) { b.push_back(info->val); });
  for (int i = 0; i < n; i++) {
    EXPECT_EQ(i, b[i]);
  }
}