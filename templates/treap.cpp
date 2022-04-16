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

struct Node {
  uint priority;
  int sz, id, a, max_a;
  Node *lch, *rch;

  Node(int id_, int a_)
      : lch(nullptr), rch(nullptr), sz(1), id(id_), a(a_), max_a(a_) {
    priority = rng();
  }

  Node* Update() {
    max_a = a;
    sz = 1;
    if (lch) {
      max_a = max(max_a, lch->max_a);
      sz += lch->sz;
    }
    if (rch) {
      max_a = max(max_a, rch->max_a);
      sz += rch->sz;
    }
    return this;
  }

  void Output() {
    if (lch) lch->Output();
    cout << id << " ";
    if (rch) rch->Output();
  }
};

Node* Merge(Node* x, Node* y) {
  if (x == nullptr) return y;
  if (y == nullptr) return x;
  if (x->priority > y->priority) {
    x->rch = Merge(x->rch, y);
    return x->Update();
  } else {
    y->lch = Merge(x, y->lch);
    return y->Update();
  }
}

pair<Node*, Node*> Split(Node* x, int pos) {
  if (x == nullptr) return {nullptr, nullptr};
  int left = x->lch ? x->lch->sz : 0;
  if (pos <= left) {
    auto [y, z] = Split(x->lch, pos);
    x->lch = nullptr;
    x->Update();
    return {y, Merge(z, x)};
  } else if (pos > left + 1) {
    pos -= left + 1;
    auto [y, z] = Split(x->rch, pos);
    x->rch = nullptr;
    x->Update();
    return {Merge(x, y), z};
  } else {
    auto y = x->rch;
    x->rch = nullptr;
    x->Update();
    return {x, y};
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
