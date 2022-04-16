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
#define debug(...) 19980723;
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

// --------------------------- xindubawukong ---------------------------

const int kMaxXY = 10000000;
const int inf = 1e8;

struct SplayNode {
  pair<int, int> info;  // y, id
  int min_id;
  SplayNode *lch, *rch, *fa;

  SplayNode() : lch(nullptr), rch(nullptr), fa(nullptr) {}

  void Update() {
    min_id = info.second;
    if (lch) {
      min_id = min(min_id, lch->min_id);
    }
    if (rch) {
      min_id = min(min_id, rch->min_id);
    }
  }
};

struct SplayTree {
  SplayNode* root;
  SplayTree() : root(nullptr) {}

  void Zig(SplayNode* t) {
    auto y = t->fa;
    t->fa = y->fa;
    if (y->fa) {
      if (y == y->fa->lch) {
        y->fa->lch = t;
      } else {
        y->fa->rch = t;
      }
    }
    y->lch = t->rch;
    if (t->rch) t->rch->fa = y;
    t->rch = y;
    y->fa = t;
    y->Update();
    t->Update();
  }

  void Zag(SplayNode* t) {
    auto y = t->fa;
    t->fa = y->fa;
    if (y->fa) {
      if (y == y->fa->lch) {
        y->fa->lch = t;
      } else {
        y->fa->rch = t;
      }
    }
    y->rch = t->lch;
    if (t->lch) t->lch->fa = y;
    t->lch = y;
    y->fa = t;
    y->Update();
    t->Update();
  }

  void Splay(SplayNode* t, SplayNode* p = nullptr) {
    assert(t);
    while (t->fa != p) {
      auto y = t->fa;
      if (y->fa == p) {
        if (t == y->lch)
          Zig(t);
        else
          Zag(t);
      } else {
        if (y == y->fa->lch) {
          if (t == y->lch)
            Zig(y), Zig(t);
          else
            Zag(t), Zig(t);
        } else {
          if (t == y->rch)
            Zag(y), Zag(t);
          else
            Zig(t), Zag(t);
        }
      }
    }
    t->Update();
    if (!p) root = t;
  }

  void Insert(int y, int id) {
    auto t = new SplayNode();
    t->info = {y, id};
    t->min_id = id;
    if (!root) {
      root = t;
      return;
    }
    SplayNode *p = root, *q = nullptr;
    while (p) {
      q = p;
      if (t->info < p->info) {
        p = p->lch;
      } else {
        p = p->rch;
      }
    }
    t->fa = q;
    if (t->info < q->info) {
      q->lch = t;
    } else {
      q->rch = t;
    }
    Splay(t);
  }

  SplayNode* SearchLagerOrEqual(int y, int id) {
    pair<int, int> info = {y, id};
    SplayNode *p = root, *res = nullptr;
    while (p) {
      if (p->info >= info) {
        res = p;
        p = p->lch;
      } else {
        p = p->rch;
      }
    }
    Splay(res);
    return res;
  }

  SplayNode* SearchSmallerOrEqual(int y, int id) {
    pair<int, int> info = {y, id};
    SplayNode *p = root, *res = nullptr;
    while (p) {
      if (p->info <= info) {
        res = p;
        p = p->rch;
      } else {
        p = p->lch;
      }
    }
    Splay(res);
    return res;
  }

  void Delete(int y, int id) {
    auto t = SearchLagerOrEqual(y, id);
    Splay(t);
    if (!t->lch) {
      root = t->rch;
      if (root) root->fa = nullptr;
    } else if (!t->rch) {
      root = t->lch;
      if (root) root->fa = nullptr;
    } else {
      auto p = t->lch;
      while (p->rch) p = p->rch;
      Splay(p, t);
      p->rch = t->rch;
      if (t->rch) t->rch->fa = p;
      p->fa = nullptr;
      p->Update();
      root = p;
    }
  }

  int GetMinId(int y1, int y2) {
    auto a = SearchSmallerOrEqual(y1, -inf);
    auto b = SearchLagerOrEqual(y2, inf);
    Splay(a);
    Splay(b, a);
    auto t = b->lch;
    if (!t) return inf;
    return t->min_id;
  }
};

struct SegmentTreeNode {
  int l, r;
  SplayTree tree;
  SegmentTreeNode *lch, *rch;

  SegmentTreeNode(int l_, int r_) : l(l_), r(r_), lch(nullptr), rch(nullptr) {}
};

SegmentTreeNode* Insert(SegmentTreeNode* x, int l, int r, int posx, int posy,
                        int id) {
  if (!x) {
    x = new SegmentTreeNode(l, r);
    x->tree.Insert(-1, inf);
    x->tree.Insert(kMaxXY + 1, inf);
  }
  if (l <= posx && posx <= r) {
    x->tree.Insert(posy, id);
  }
  if (l == r) return x;
  int m = (l + r) / 2;
  if (posx <= m) {
    x->lch = Insert(x->lch, l, m, posx, posy, id);
  } else {
    x->rch = Insert(x->rch, m + 1, r, posx, posy, id);
  }
  return x;
}

void Delete(SegmentTreeNode* x, int posx, int posy, int id) {
  if (!x) return;
  if (x->l <= posx && posx <= x->r) {
    x->tree.Delete(posy, id);
  }
  if (x->l == x->r) return;
  int m = (x->l + x->r) / 2;
  if (posx <= m) {
    Delete(x->lch, posx, posy, id);
  } else {
    Delete(x->rch, posx, posy, id);
  }
}

int GetMinId(SegmentTreeNode* x, int x1, int x2, int y1, int y2) {
  if (!x) return inf;
  if (x1 <= x->l && x->r <= x2) {
    return x->tree.GetMinId(y1, y2);
  }
  int m = (x->l + x->r) / 2;
  int res = inf;
  if (x1 <= m) {
    res = min(res, GetMinId(x->lch, x1, x2, y1, y2));
  }
  if (x2 > m) {
    res = min(res, GetMinId(x->rch, x1, x2, y1, y2));
  }
  return res;
}

void Main() {}

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
