vector<int> dep(n);
  vector<vector<int>> fa(n, vector<int>(logn));
  vector<vector<int>> maxz(n, vector<int>(logn, -inf));
  vector<int> que = {0};
  fa[0][0] = 0;
  dep[0] = 0;
  for (int i = 0; i < que.size(); i++) {
    int x = que[i];
    for (auto [y, z] : go[x]) {
      if (y == fa[x][0]) continue;
      fa[y][0] = x;
      maxz[y][0] = z;
      dep[y] = dep[x] + 1;
      que.push_back(y);
    }
  }
  for (int j = 1; j < logn; j++) {
    for (int i = 0; i < n; i++) {
      fa[i][j] = fa[fa[i][j - 1]][j - 1];
      maxz[i][j] = max(maxz[i][j - 1], maxz[fa[i][j - 1]][j - 1]);
    }
  }
  auto GetLca = [&](int x, int y) {
    if (dep[x] < dep[y]) swap(x, y);
    if (dep[x] > dep[y]) {
      for (int j = logn - 1; j >= 0; j--) {
        if (dep[fa[x][j]] >= dep[y]) {
          x = fa[x][j];
        }
      }
    }
    if (x == y) return x;
    for (int j = logn - 1; j >= 0; j--) {
      if (fa[x][j] != fa[y][j]) {
        x = fa[x][j];
        y = fa[y][j];
      }
    }
    return fa[x][0];
  };