vector<bool> GetCutEdges(int n, int m, auto& go) {
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

vector<bool> GetCutVertices(int n, int m, auto& go) {
  vector<int> low(n), dfn(n);
  vector<bool> vt(n);
  int ts = 0;
  vector<bool> cut(n);
  auto Dfs = [&](auto self, int x, int from) -> void {
    dfn[x] = low[x] = ++ts;
    vt[x] = true;
    int num = 0;
    for (int y : go[x]) {
      if (y == from) continue;
      if (!vt[y]) {
        self(self, y, x);
        low[x] = min(low[x], low[y]);
        if (low[y] >= dfn[x]) num++;
      } else {
        low[x] = min(low[x], dfn[y]);
      }
    }
    if ((x == 0 && num > 1) || (x > 0 && num > 0)) {
      cut[x] = true;
    }
  };
  for (int i = 0; i < n; i++) {
    if (!vt[i]) {
      Dfs(Dfs, i, -1);
    }
  }
  return cut;
}
