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

// --------------------------- xindubawukong ---------------------------

std::mt19937 rng(19980723);

struct Node {
  uint priority;
  int sz, val, num;
  Node *lch, *rch;

  Node(int val) : lch(nullptr), rch(nullptr), sz(1), val(val), num(1) {
    priority = rng();
  }

  Node(Node* x)
      : lch(x->lch),
        rch(x->rch),
        sz(x->sz),
        val(x->val),
        num(x->num),
        priority(x->priority) {}

  Node* Update() {
    sz = num;
    if (lch) {
      sz += lch->sz;
    }
    if (rch) {
      sz += rch->sz;
    }
    return this;
  }

  void Output(vector<int>& a) {
    if (lch) lch->Output(a);
    for (int i = 0; i < num; i++) {
      a.push_back(val);
    }
    if (rch) rch->Output(a);
  }

  vector<int> Output() {
    vector<int> a;
    Output(a);
    return a;
  }
};

Node* Merge(Node* x, Node* y) {
  if (x == nullptr) return y;
  if (y == nullptr) return x;
  if (x->priority > y->priority) {
    Node* z = new Node(x);
    z->rch = Merge(x->rch, y);
    return z->Update();
  } else {
    Node* z = new Node(y);
    z->lch = Merge(x, y->lch);
    return z->Update();
  }
}

Node* Merge(Node* x, Node* y, Node* z) {
  if (x == nullptr) return Merge(y, z);
  if (y == nullptr) return Merge(x, z);
  if (z == nullptr) return Merge(x, y);
  if (y->priority > x->priority && y->priority > z->priority) {
    Node* w = new Node(y);
    w->lch = x;
    w->rch = z;
    return w->Update();
  } else if (x->priority > z->priority) {
    Node* w = new Node(x);
    w->rch = Merge(x->rch, y, z);
    return w->Update();
  } else {
    Node* w = new Node(z);
    w->lch = Merge(x, y, z->lch);
    return w->Update();
  }
}

tuple<Node*, Node*, Node*> Split(Node* x, int val) {
  if (x == nullptr) {
    return {nullptr, nullptr, nullptr};
  }
  if (x->val == val) {
    Node* y = new Node(x);
    y->lch = y->rch = nullptr;
    return {x->lch, y->Update(), x->rch};
  }
  if (val < x->val) {
    auto [w, y, z] = Split(x->lch, val);
    Node* v = new Node(x);
    v->lch = nullptr;
    return {w, y, Merge(z, v->Update())};
  } else {
    auto [w, y, z] = Split(x->rch, val);
    Node* v = new Node(x);
    v->rch = nullptr;
    return {Merge(v->Update(), w), y, z};
  }
}

Node* Insert(Node* root, int val) {
  auto [x, y, z] = Split(root, val);
  if (y) {
    y->num++, y->sz++;
    return Merge(x, y, z);
  } else {
    y = new Node(val);
    return Merge(x, y, z);
  }
}

Node* Delete(Node* root, int val) {
  auto [x, y, z] = Split(root, val);
  if (!y) return Merge(x, z);
  y->num--, y->sz--;
  if (y->num == 0) {
    delete y;
    return Merge(x, z);
  }
  return Merge(x, y, z);
}

int GetRank(Node* root, int val) {
  Node* x = root;
  int less = 0;
  while (x) {
    if (val < x->val) {
      x = x->lch;
    } else {
      less += x->lch ? x->lch->sz : 0;
      if (val == x->val) break;
      less += x->num;
      x = x->rch;
    }
  }
  return less + 1;
}

int GetKth(Node* x, int k) {
  int left = x->lch ? x->lch->sz : 0;
  if (k <= left) {
    return GetKth(x->lch, k);
  }
  k -= left;
  if (k <= x->num) {
    return x->val;
  }
  k -= x->num;
  return GetKth(x->rch, k);
}

int GetPrev(Node* root, int val) {
  auto [x, y, z] = Split(root, val);
  if (x) {
    while (x->rch) x = x->rch;
    return x->val;
  } else {
    return INT_MIN + 1;
  }
}

int GetSucc(Node* root, int val) {
  auto [x, y, z] = Split(root, val);
  if (z) {
    while (z->lch) z = z->lch;
    return z->val;
  } else {
    return INT_MAX;
  }
}

int FindLastLarger(Node* x, int a) {
  if (x == nullptr) return 0;
  int left = x->lch ? x->lch->sz : 0;
  if (x->rch && x->rch->max_a > a) {
    return left + 1 + FindLastLarger(x->rch, a);
  }
  if (x->a > a) return left + 1;
  return FindLastLarger(x->lch, a);
}

Node* Insert(Node* root, int pos, int id, int a) {
  Node* x = new Node(id, a);
  auto [y, z] = Split(root, pos);
  return Merge(Merge(y, x), z);
}

void Main() {}

int main() {
  // std::ios::sync_with_stdio(false);
  // std::cin.tie(nullptr);
#ifdef LOCAL
  freopen("../problem_A/A.in", "r", stdin);
  // freopen("../problem_A/A.out", "w", stdout);
#endif
  Main();
  return 0;
}
