class Dsu {
 public:
  vector<int> fa;
  int cnt;

  Dsu(int n) {
    fa.resize(n);
    for (int i = 0; i < n; i++) fa[i] = i;
    cnt = n;
  }

  int GetFa(int x) {
    if (fa[x] == x) return x;
    return fa[x] = GetFa(fa[x]);
  }

  void Merge(int x, int y) {
    x = GetFa(x);
    y = GetFa(y);
    if (x != y) {
      fa[x] = y;
      cnt--;
    }
  }
};