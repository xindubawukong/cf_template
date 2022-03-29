void Dfs(int x) {
  dfn[x] = low[x] = ++timestamp;
  visit[x] = true;
  sta.push(x);
  for (int y : go[x]) {
    if (!visit[y]) {
      Dfs(y);
      low[x] = min(low[x], low[y]);
    } else if (belong[y] == 0) {
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
}