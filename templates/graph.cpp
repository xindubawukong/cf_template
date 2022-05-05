// cut edges
vector<bool> GetCut(int n, int m, auto& go) {
  vector<int> low(n), dfn(n);
  vector<bool> vt(n);
  int ts = 0;
  vector<bool> cut(m);
  auto Dfs = [&](auto self, int x, int from) -> void {
    dfn[x] = low[x] = ++ts;
    vt[x] = true;
    for (auto [y, id] : go[x]) {
      if (id == from) continue;
      if (!vt[y]) {
        self(self, y, id);
        low[x] = min(low[x], low[y]);
        if (low[y] > dfn[x]) {
          cut[id] = true;
        }
      } else {
        low[x] = min(low[x], dfn[y]);
      }
    }
  };
  for (int i = 0; i < n; i++) {
    if (!vt[i]) {
      Dfs(Dfs, i, -1);
    }
  }
  return cut;
}