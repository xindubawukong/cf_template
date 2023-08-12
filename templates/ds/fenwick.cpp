struct Fenwick {
  int n;
  vector<int> a;
  Fenwick(int n_) : n(n_) { a.resize(n); }
  void Change(int x, int dx) {
    while (x < n) {
      a[x] += dx;
      x |= (x + 1);
    }
  }
  int GetSum(int x) {
    int res = 0;
    while (x >= 0) {
      res += a[x];
      x = (x & (x + 1)) - 1;
    }
    return res;
  }
  int RangeSum(int l, int r) {
    auto res = GetSum(r);
    if (l > 0) res -= GetSum(l - 1);
    return res;
  }
};