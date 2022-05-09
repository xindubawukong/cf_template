#if __APPLE__ && __clang__
#define LOCAL
#endif

#include <algorithm>
#include <any>
#include <array>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef LOCAL
#include "../debug.h"
#else
#define debug(...) 19980723
#endif

using namespace std;

using int64 = long long;
#if _WIN64 || __x86_64__
using int128 = __int128_t;
#endif
using uint = unsigned int;
using uint64 = unsigned long long;

mt19937 rng(19980723);
mt19937_64 rng64(19980723);

// ----------------------------------------------------------------------

struct SegmentTree {
  struct Node {
    int64 sum_a, sum_b, add_a, add_b;
    Node *lch, *rch;

    Node()
        : sum_a(0), sum_b(0), add_a(0), add_b(0), lch(nullptr), rch(nullptr) {}

    void Add(int l, int r, int64 da, int64 db) {
      add_a += da;
      add_b += db;
      sum_a += da * (r - l + 1);
      sum_b += db * (r - l + 1);
    }

    void PushDown(int l, int r) {
      if (add_a == 0 && add_b == 0) return;
      if (!lch) lch = new Node();
      if (!rch) rch = new Node();
      int m = l + (r - l) / 2;
      lch->Add(l, m, add_a, add_b);
      rch->Add(m + 1, r, add_a, add_b);
      add_a = add_b = 0;
    }

    void Update() {
      sum_a = sum_b = 0;
      if (lch) {
        sum_a += lch->sum_a;
        sum_b += lch->sum_b;
      }
      if (rch) {
        sum_a += rch->sum_a;
        sum_b += rch->sum_b;
      }
    }
  };

  int l_range, r_range;
  Node* root;

  SegmentTree(int l, int r) : l_range(l), r_range(r), root(nullptr) {}

  Node* Change(Node* x, int l, int r, int from, int to, int64 da, int64 db) {
    if (!x) {
      x = new Node();
    }
    if (from <= l && r <= to) {
      x->Add(l, r, da, db);
      return x;
    }
    x->PushDown(l, r);
    int m = l + (r - l) / 2;
    if (from <= m) {
      x->lch = Change(x->lch, l, m, from, to, da, db);
    }
    if (to > m) {
      x->rch = Change(x->rch, m + 1, r, from, to, da, db);
    }
    x->Update();
    return x;
  }

  void Change(int from, int to, int64 da, int64 db) {
    root = Change(root, l_range, r_range, from, to, da, db);
  }

  pair<int64, int64> GetSum(Node* x, int l, int r, int from, int to) {
    if (!x) return {0, 0};
    if (from <= l && r <= to) {
      return {x->sum_a, x->sum_b};
    }
    x->PushDown(l, r);
    int m = l + (r - l) / 2;
    pair<int64, int64> res = {0, 0};
    if (from <= m) {
      auto [a, b] = GetSum(x->lch, l, m, from, to);
      res.first += a;
      res.second += b;
    }
    if (to > m) {
      auto [a, b] = GetSum(x->rch, m + 1, r, from, to);
      res.first += a;
      res.second += b;
    }
    return res;
  }

  pair<int64, int64> GetSum(int from, int to) {
    return GetSum(root, l_range, r_range, from, to);
  }
};

void Main() {
  int n, m;
  cin >> n >> m;
  SegmentTree tree(1, n);
  for (int i = 1; i <= n; i++) {
    int x;
    cin >> x;
    tree.Change(i, i, x);
  }
  for (int i = 0; i < m; i++) {
    int op, l, r, k;
    cin >> op >> l >> r;
    if (op == 1) {
      cin >> k;
      tree.Change(l, r, k);
    } else {
      cout << tree.GetSum(l, r) << "\n";
    }
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
#ifdef LOCAL
  freopen("../problem_A/A.in", "r", stdin);
  // freopen("../problem_A/A.out", "w", stdout);
#endif
  Main();
  return 0;
}
