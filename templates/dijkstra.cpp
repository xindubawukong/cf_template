#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

const long long inf = 1e60;

struct Edge {
  int y, w;
  Edge(int y_, int w_) : y(y_), w(w_) {}
};

int n, m;
vector<long long> dist;
vector<int> from;
vector<vector<Edge>> go;

void Dijkstra(int s, int t) {
  for (int i = 1; i <= n; i++) {
    dist[i] = inf;
  }
  dist[s] = 0;
  vector<bool> visit(n + 1);
  priority_queue<pair<int, int>> q;
  q.push(make_pair(0, s));
  while (!q.empty()) {
    int x = q.top().second;
    q.pop();
    visit[x] = true;
    for (Edge& e : go[x]) {
      int y = e.y, w = e.w;
      if (visit[y]) continue;
      long long d = dist[x] + w;
      if (d < dist[y]) {
        dist[y] = d;
        from[y] = x;
        q.push(make_pair(-d, y));
      }
    }
  }
}

int main() {
  // freopen("../input.txt", "r", stdin);
  scanf("%d%d", &n, &m);
  go.resize(n + 1);
  int x, y, w;
  for (int i = 0; i < m; i++) {
    scanf("%d%d%d", &x, &y, &w);
    go[x].push_back(Edge(y, w));
    go[y].push_back(Edge(x, w));
  }
  dist.resize(n + 1);
  from.resize(n + 1);
  Dijkstra(1, n);
  if (dist[n] == inf) {
    cout << -1 << endl;
  } else {
    vector<int> ans;
    int now = n;
    while (now != 1) {
      ans.push_back(now);
      now = from[now];
    }
    ans.push_back(1);
    for (int i = ans.size() - 1; i >= 0; i--) {
      printf("%d ", ans[i]);
    }
    printf("\n");
  }
  return 0;
}