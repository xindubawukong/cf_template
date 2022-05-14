#if __APPLE__ && __clang__
#define LOCAL
#endif

#include <algorithm>
#include <any>
#include <array>
#include <bit>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef LOCAL
#include "../debug.h"
#else
#define debug(...) 19980723
#endif

using namespace std;

using int64 = long long;
#if _WIN64 || __x86_64__
using int128 = __int128_t;
#endif
using uint = unsigned int;
using uint64 = unsigned long long;

mt19937 rng(19980723);
mt19937_64 rng64(19980723);

// ----------------------------------------------------------------------

struct MaxFlow {
  struct Edge {
    int y;
    int64 cap;
    Edge* back;
  };

  int n;
  vector<vector<Edge*>> go;

  MaxFlow(int n_) : n(n_) { go.resize(n); }

  void AddEdge(int x, int y, int64 cap) {
    assert(0 <= x && x < n && 0 <= y && y < n);
    auto AddRawEdge = [&](int x, int y, int64 cap) {
      auto e = new Edge();
      e->y = y;
      e->cap = cap;
      go[x].push_back(e);
      return e;
    };
    auto e1 = AddRawEdge(x, y, cap);
    auto e2 = AddRawEdge(y, x, 0);
    e1->back = e2;
    e2->back = e1;
  }

  int64 Solve(int s, int t) {
    assert(0 <= s && s < n && 0 <= t && t < n);
    vector<int> dep(n);
    vector<int> cur(n);
    auto Bfs = [&]() -> bool {
      std::fill(dep.begin(), dep.end(), -1);
      dep[s] = 0;
      queue<int> q;
      q.push(s);
      while (!q.empty()) {
        int x = q.front();
        q.pop();
        for (auto e : go[x]) {
          if (e->cap > 0 && dep[e->y] == -1) {
            dep[e->y] = dep[x] + 1;
            q.push(e->y);
          }
        }
      }
      return dep[t] != -1;
    };
    auto Dfs = [&](auto self, int x, int64 flow) -> int64 {
      if (x == t) return flow;
      int64 res = 0;
      for (int& i = cur[x]; i < go[x].size(); i++) {
        auto& [y, cap, back] = *go[x][i];
        if (cap == 0 || dep[y] != dep[x] + 1) continue;
        int64 df = self(self, y, min(cap, flow));
        flow -= df;
        cap -= df;
        back->cap += df;
        res += df;
        if (flow == 0) break;
      }
      if (res == 0) dep[x] = 1e8;
      return res;
    };
    int64 ans = 0;
    while (Bfs()) {
      std::fill(cur.begin(), cur.end(), 0);
      ans += Dfs(Dfs, s, 1e18);
    }
    return ans;
  }
};

struct MinCostMaxFlow {
  struct Edge {
    int y;
    int64 cap, cost;
    Edge* back;
  };
  int n;
  vector<vector<Edge*>> go;

  MinCostMaxFlow(int n_) : n(n_) { go.resize(n); }

  void AddEdge(int x, int y, int64 cap, int64 cost) {
    assert(0 <= x && x < n && 0 <= y && y < n);
    auto AddRawEdge = [&](int x, int y, int64 cap, int64 cost) {
      auto e = new Edge();
      e->y = y;
      e->cap = cap;
      e->cost = cost;
      go[x].push_back(e);
      return e;
    };
    auto e1 = AddRawEdge(x, y, cap, cost);
    auto e2 = AddRawEdge(y, x, 0, -cost);
    e1->back = e2;
    e2->back = e1;
  }

  pair<int64, int64> Solve(int s, int t) {
    assert(0 <= s && s < n && 0 <= t && t < n);
    vector<Edge*> pre(n);
    auto Spfa = [&]() -> bool {
      std::fill(pre.begin(), pre.end(), nullptr);
      vector<int64> dist(n, 1e18);
      vector<bool> inq(n, false);
      dist[s] = 0;
      inq[s] = true;
      queue<int> q;
      q.push(s);
      while (!q.empty()) {
        int x = q.front();
        q.pop();
        inq[x] = false;
        for (auto e : go[x]) {
          int y = e->y;
          if (e->cap > 0 && dist[y] > dist[x] + e->cost) {
            pre[y] = e;
            dist[y] = dist[x] + e->cost;
            if (!inq[y]) {
              inq[y] = true;
              q.push(y);
            }
          }
        }
      }
      return dist[t] < 1e17;
    };
    auto Calc = [&]() -> pair<int64, int64> {
      int64 flow = 1e18;
      for (auto e = pre[t]; e; e = pre[e->back->y]) {
        flow = min(flow, e->cap);
      }
      int64 cost = 0;
      for (auto e = pre[t]; e; e = pre[e->back->y]) {
        cost += flow * e->cost;
        e->cap -= flow;
        e->back->cap += flow;
      }
      return {flow, cost};
    };
    int64 flow = 0, cost = 0;
    while (Spfa()) {
      auto [f, c] = Calc();
      flow += f, cost += c;
    }
    return {flow, cost};
  }
};


void Main() {
  int n, m, s, t;
  cin >> n >> m >> s >> t;
  s--, t--;
  MaxFlow mf(n);
  for (int i = 0; i < m; i++) {
    int x, y, z;
    cin >> x >> y >> z;
    x--, y--;
    mf.AddEdge(x, y, z);
  }
  auto ans = mf.Solve(s, t);
  cout << ans << endl;
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
