vector<int> GetSCC(int n, auto& go) {
  vector<int> dfn(n), low(n), belong(n, -1);
  vector<bool> vt(n);
  stack<int> sta;
  int ts = 0;
  auto Dfs = [&](auto self, int x) -> void {
    dfn[x] = low[x] = ++ts;
    vt[x] = true;
    sta.push(x);
    for (int y : go[x]) {
      if (!vt[y]) {
        self(self, y);
        low[x] = min(low[x], low[y]);
      } else if (belong[y] == -1) {
        low[x] = min(low[x], dfn[y]);
      }
    }
    if (low[x] == dfn[x]) {
      while (!sta.empty()) {
        int y = sta.top();
        belong[y] = x;
        sta.pop();
        if (y == x) break;
      }
    }
  };
  for (int i = 0; i < n; i++) {
    if (!vt[i]) {
      Dfs(Dfs, i);
    }
  }
  return belong;
}