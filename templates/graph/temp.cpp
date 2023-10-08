
基环有向图找环

vector<int> vt(n, -1);
vector<vector<int>> circles;
vector<int> sta;
function<void(int, int)> Dfs = [&](int u, int ts) -> void {
  vt[u] = ts;
  sta.push_back(u);
  int v = go[u];
  if (vt[v] != -1) {
    if (vt[v] != ts) return;
    vector<int> circle;
    do {
      circle.push_back(sta.back());
      sta.pop_back();
    } while (circle.back() != v);
    reverse(circle.begin(), circle.end());
    circles.push_back(circle);
  } else {
    Dfs(v, ts);
  }
};
for (int i = 0; i < n; i++) {
  if (vt[i] == -1) {
    sta.clear();
    Dfs(i, i);
  }
}
for (auto& circle : circles) debug(circle);
