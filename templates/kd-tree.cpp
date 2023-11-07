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

// https://codeforces.com/group/BXTLKO2eOg/contest/484219/problem/A

#include <bits/stdc++.h>
#ifdef LOCAL
#include "debug.h"
#else
#define debug(...)
#endif
using int64 = long long;
using uint = unsigned int;
using uint64 = unsigned long long;
bool ckmin(auto& a, auto b) { return b < a ? a = b, 1 : 0; }
bool ckmax(auto& a, auto b) { return b > a ? a = b, 1 : 0; }
using namespace std;

using rea = double;

template <int D>
struct KDTree {
  using Point = array<rea, D>;

  struct Node {
    Node* lch;
    Node* rch;
    Point p;
    rea minn[D], maxx[D];
    Node() : lch(nullptr), rch(nullptr) {}
    void Update() {
      for (int i = 0; i < D; i++) minn[i] = maxx[i] = p[i];
      if (lch) {
        for (int i = 0; i < D; i++) {
          ckmin(minn[i], lch->minn[i]);
          ckmax(maxx[i], lch->maxx[i]);
        }
      }
      if (rch) {
        for (int i = 0; i < D; i++) {
          ckmin(minn[i], rch->minn[i]);
          ckmax(maxx[i], rch->maxx[i]);
        }
      }
    }
    rea Try(Point a) {
      rea t = 0;
      for (int i = 0; i < D; i++) {
        if (a[i] < minn[i]) t += (a[i] - minn[i]) * (a[i] - minn[i]);
        if (a[i] > maxx[i]) t += (a[i] - maxx[i]) * (a[i] - maxx[i]);
      }
      return sqrt(t);
    }
  };

  Node* root;
  array<function<bool(const Point&, const Point&)>, D> cmp;

  KDTree() {
    for (int i = 0; i < D; i++) {
      cmp[i] = [=](const Point& a, const Point& b) { return a[i] < b[i]; };
    }
  }

  Node* BuildTree(vector<Point>& ps, int l, int r) {
    if (l > r) return nullptr;
    double var[D];
    for (int d = 0; d < D; d++) {
      double t1 = 0, t2 = 0;
      for (int i = l; i <= r; i++) {
        t1 += 1.0 * ps[i][d] * ps[i][d];
        t2 += ps[i][d];
      }
      var[d] = t1 - t2 * t2 / (r - l + 1);
    }
    int d = max_element(var, var + D) - var;
    int mid = (l + r) / 2;
    nth_element(ps.begin() + l, ps.begin() + mid, ps.begin() + r + 1, cmp[d]);
    Node* x = new Node();
    x->p = ps[mid];
    x->lch = BuildTree(ps, l, mid - 1);
    x->rch = BuildTree(ps, mid + 1, r);
    x->Update();
    return x;
  }
};

template <int D>
rea Dist(const array<rea, D>& a, const array<rea, D>& b) {
  rea t = 0;
  for (int i = 0; i < D; i++) t += (a[i] - b[i]) * (a[i] - b[i]);
  return sqrt(t);
}

template <int D>
void Work(int n) {
  using Point = array<rea, D>;
  vector<array<rea, D>> a(n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < D; j++) cin >> a[i][j];
  }

  KDTree<D> kdtree;
  kdtree.root = kdtree.BuildTree(a, 0, n - 1);

  int m;
  cin >> m;
  while (m--) {
    Point p;
    int k;
    cin >> k;
    for (int i = 0; i < D; i++) cin >> p[i];
    auto Cmp = [&](const Point& a, const Point& b) {
      rea d1 = Dist<D>(a, p);
      rea d2 = Dist<D>(b, p);
      return d1 < d2;
    };
    priority_queue<Point, vector<Point>, decltype(Cmp)> s(Cmp);
    auto Query = [&](auto self, KDTree<D>::Node* node) {
      if (!node) return;
      if ((int)s.size() == k && node->Try(p) >= Dist<D>(p, s.top())) return;
      s.push(node->p);
      if ((int)s.size() > k) s.pop();
      auto left = node->lch ? node->lch->Try(p) : 0;
      auto right = node->rch ? node->rch->Try(p) : 0;
      if (left < right) {
        self(self, node->lch);
        self(self, node->rch);
      } else {
        self(self, node->rch);
        self(self, node->lch);
      }
    };
    Query(Query, kdtree.root);
    cout << fixed << setprecision(10) << Dist<D>(s.top(), p) << '\n';
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, D;
  cin >> n >> D;
  if (D == 2) Work<2>(n);
  if (D == 3) Work<3>(n);
  if (D == 4) Work<4>(n);
  if (D == 5) Work<5>(n);

  return 0;
}
