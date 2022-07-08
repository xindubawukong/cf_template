#if __APPLE__ && __clang__
#define LOCAL
#endif

#if __cplusplus >= 202001L
#include <any>
#include <bit>
#include <optional>
#endif

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
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

// ----------------------------------------------------------------------

// https://www.luogu.com.cn/problem/P4148

const double alpha = 0.75;

struct Point {
  int x, y, val;
};

struct Node {
  Point p;
  int sum, size;
  int min_x, max_x, min_y, max_y;
  Node *lch, *rch;

  Node(const Point& p) : p(p), lch(nullptr), rch(nullptr) {
    sum = p.val;
    size = 1;
    min_x = max_x = p.x;
    min_y = max_y = p.y;
  }

  void Update() {
    sum = p.val;
    size = 1;
    if (lch) {
      sum += lch->sum;
      size += lch->size;
      min_x = min(min_x, lch->min_x);
      max_x = max(max_x, lch->max_x);
      min_y = min(min_y, lch->min_y);
      max_y = max(max_y, lch->max_y);
    }
    if (rch) {
      sum += rch->sum;
      size += rch->size;
      min_x = min(min_x, rch->min_x);
      max_x = max(max_x, rch->max_x);
      min_y = min(min_y, rch->min_y);
      max_y = max(max_y, rch->max_y);
    }
  }
};

const vector<function<bool(const Point& a, const Point& b)>> cmps = {
    [](const Point& a, const Point& b) { return a.x < b.x; },
    [](const Point& a, const Point& b) { return a.y < b.y; }};

Node* BuildTree(vector<Point>& a, int l, int r, int d) {
  if (l > r) return nullptr;
  int mid = (l + r) / 2;
  std::nth_element(a.begin() + l, a.begin() + mid, a.begin() + r + 1, cmps[d]);
  Node* x = new Node(a[mid]);
  x->lch = BuildTree(a, l, mid - 1, d ^ 1);
  x->rch = BuildTree(a, mid + 1, r, d ^ 1);
  x->Update();
  return x;
}

Node* Rebuild(Node* x, int d) {
  vector<Point> a;
  auto Flatten = [&](auto self, Node* x) -> void {
    a.push_back(x->p);
    if (x->lch) self(self, x->lch);
    if (x->rch) self(self, x->rch);
    delete (x);
  };
  Flatten(Flatten, x);
  return BuildTree(a, 0, a.size() - 1, d);
}

bool IsBalance(Node* x) {
  int tot = x->size;
  int a = x->lch ? x->lch->size : 0;
  int b = x->rch ? x->rch->size : 0;
  if (a > tot * alpha) return false;
  if (b > tot * alpha) return false;
  return true;
}

Node* Insert(Node* x, Point& p, int d, bool check) {
  if (!x) {
    return new Node(p);
  }
  bool rebuild = false;
  if (check && !IsBalance(x)) {
    rebuild = true;
    check = false;
  }
  if (cmps[d](p, x->p)) {
    x->lch = Insert(x->lch, p, d ^ 1, check);
  } else {
    x->rch = Insert(x->rch, p, d ^ 1, check);
  }
  x->Update();
  if (rebuild) {
    return Rebuild(x, d);
  } else {
    return x;
  }
}

int Query(Node* x, int x1, int y1, int x2, int y2) {
  if (!x) return 0;
  if (x->max_x < x1 || x->min_x > x2 || x->max_y < y1 || x->min_y > y2) {
    return 0;
  }
  if (x->min_x >= x1 && x->max_x <= x2 && x->min_y >= y1 && x->max_y <= y2) {
    return x->sum;
  }
  int res = 0;
  if (x->p.x >= x1 && x->p.x <= x2 && x->p.y >= y1 && x->p.y <= y2) {
    res += x->p.val;
  }
  res += Query(x->lch, x1, y1, x2, y2);
  res += Query(x->rch, x1, y1, x2, y2);
  return res;
}

void Main() {
  Node* root = nullptr;
  int n, op, last_ans = 0;
  cin >> n;
  while (cin >> op) {
    if (op == 1) {
      int x, y, z;
      cin >> x >> y >> z;
      x ^= last_ans, y ^= last_ans, z ^= last_ans;
      Point p = {x, y, z};
      root = Insert(root, p, 0, true);
    } else if (op == 2) {
      int x1, y1, x2, y2;
      cin >> x1 >> y1 >> x2 >> y2;
      x1 ^= last_ans, y1 ^= last_ans, x2 ^= last_ans, y2 ^= last_ans;
      last_ans = Query(root, x1, y1, x2, y2);
      cout << last_ans << endl;
    } else {
      break;
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
