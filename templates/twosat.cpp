#if __APPLE__ && __clang__
#define LOCAL
#endif

#if __cplusplus >= 202001L
#include <any>
#include <bit>
#include <optional>
#endif

#include <algorithm>
#include <array>
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
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
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

// ----------------------------------------------------------------------

vector<int> GetSCC(int n, auto& go) {
  vector<int> dfn(n), low(n), belong(n, -1);
  vector<bool> vt(n);
  stack<int> sta;
  int ts = 0, cnt = 0;
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
        belong[y] = cnt;
        sta.pop();
        if (y == x) break;
      }
      cnt++;
    }
  };
  for (int i = 0; i < n; i++) {
    if (!vt[i]) {
      Dfs(Dfs, i);
    }
  }
  return belong;
}

struct TwoSat {
  int n;
  vector<vector<int>> go;

  TwoSat(int n_) : n(n_) { go.resize(n * 2); }

  void Add(int i, bool x, int j, bool y) {
    assert(0 <= i && i < n && 0 <= j && j < n);
    go[i * 2 + !x].push_back(j * 2 + y);
    go[j * 2 + !y].push_back(i * 2 + x);
  }

  pair<bool, vector<bool>> Solve() {
    auto belong = GetSCC(n * 2, go);
    vector<bool> ans(n);
    for (int i = 0; i < n; i++) {
      if (belong[i * 2] == belong[i * 2 + 1]) {
        return {false, {}};
      } else if (belong[i * 2] < belong[i * 2 + 1]) {
        ans[i] = false;
      } else {
        ans[i] = true;
      }
    }
    return {true, ans};
  }
};

void Main() {
  int n, m;
  cin >> n >> m;
  TwoSat ts(n);
  for (int t = 0; t < m; t++) {
    int i, j, a, b;
    cin >> i >> a >> j >> b;
    i--, j--;
    ts.Add(i, a, j, b);
  }
  auto [ok, ans] = ts.Solve();
  if (!ok) {
    cout << "IMPOSSIBLE\n";
  } else {
    cout << "POSSIBLE\n";
    for (int i = 0; i < n; i++) {
      cout << ans[i] << " \n"[i == n - 1];
    }
  }
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
