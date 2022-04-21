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
    int64 sum, tag;
    Node *lch, *rch;

    Node() : sum(0), tag(0), lch(nullptr), rch(nullptr) {}

    void Add(int64 val, int l, int r) {
      tag += val;
      sum += val * (r - l + 1);
    }

    void PushDown(int l, int r) {
      if (tag == 0) return;
      int m = l + (r - l) / 2;
      if (lch) {
        lch->Add(tag, l, m);
      }
      if (rch) {
        rch->Add(tag, m + 1, r);
      }
      tag = 0;
    }

    void Update() {
      sum = 0;
      if (lch) {
        sum += lch->sum;
      }
      if (rch) {
        sum += rch->sum;
      }
    }
  };

  int l_range, r_range;
  Node* root;

  SegmentTree(int l, int r) : l_range(l), r_range(r), root(nullptr) {}

  Node* Change(Node* x, int l, int r, int from, int to, int64 val) {
    if (!x) {
      x = new Node();
    }
    if (from <= l && r <= to) {
      x->Add(val, l, r);
      return x;
    }
    x->PushDown(l, r);
    int m = l + (r - l) / 2;
    if (from <= m) {
      x->lch = Change(x->lch, l, m, from, to, val);
    }
    if (to > m) {
      x->rch = Change(x->rch, m + 1, r, from, to, val);
    }
    x->Update();
    return x;
  }

  void Change(int from, int to, int64 val) {
    root = Change(root, l_range, r_range, from, to, val);
  }

  int64 GetSum(Node* x, int l, int r, int from, int to) {
    if (!x) return 0;
    if (from <= l && r <= to) {
      return x->sum;
    }
    x->PushDown(l, r);
    int m = l + (r - l) / 2;
    int64 res = 0;
    if (from <= m) {
      res += GetSum(x->lch, l, m, from, to);
    }
    if (to > m) {
      res += GetSum(x->rch, m + 1, r, from, to);
    }
    return res;
  }

  int64 GetSum(int from, int to) {
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
