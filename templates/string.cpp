#if __APPLE__ && __clang__
#define LOCAL
#endif

#include <algorithm>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef LOCAL
#include "debug.h"
#endif

using namespace std;

using float64 = long double;
using int64 = long long;
#if _WIN64 || __x86_64__
using int128 = __int128_t;
#endif
using uint64 = unsigned long long;

// --------------------------- xindubawukong ---------------------------

vector<int> Manacher(const string& s) {
  string t;
  vector<int> p;
  t.reserve(s.length() * 2 + 1);
  t += '!';
  for (int i = 0; i < s.length(); i++) {
    t += s[i];
    if (i < s.length() - 1) t += '#';
  }
  t += '@';
  p.resize(t.length());
  p[0] = 0;
  int now = 0, right = 0;
  for (int i = 1; i < p.size() - 1; i++) {
    if (right > i) {
      p[i] = min(p[now * 2 - i], right - i);
    } else {
      p[i] = 0;
    }
    while (t[i - p[i] - 1] == t[i + p[i] + 1]) p[i]++;
    if (i + p[i] > right) {
      now = i;
      right = i + p[i];
    }
  }
  return p;
}

// suffix array problem: https://codeforces.com/contest/19/problem/C

struct SuffixArray {
  int n;
  vector<int> a, sa, rank, height;

  void Prepare() {
    vector<int> b = a;
    sort(b.begin(), b.end());
    for (int i = 0; i < n; i++) {
      a[i] = lower_bound(b.begin(), b.end(), a[i]) - b.begin();
    }
  }

  void Resort(vector<array<int, 2>>& key) {
    for (int i = 0; i < n; i++) key[i][0]++, key[i][1]++;
    vector<int> b(n + 1), sum(n + 2), at(n);
    for (int i = 0; i < n; i++) at[i] = b[key[i][1]]++;
    for (int i = 0; i <= n; i++) sum[i + 1] = sum[i] + b[i];
    for (int i = 0; i < n; i++) sa[sum[key[i][1]] + at[i]] = i;
    for (int i = 0; i <= n; i++) b[i] = 0;
    for (int i = 0; i < n; i++) at[sa[i]] = b[key[sa[i]][0]]++;
    for (int i = 0; i <= n; i++) sum[i + 1] = sum[i] + b[i];
    for (int i = 0; i < n; i++) sa[sum[key[i][0]] + at[i]] = i;
    for (int i = 0, r = 0; i < n; i++) {
      if (i > 0 && key[sa[i]] > key[sa[i - 1]]) r++;
      rank[sa[i]] = r;
    }
  }

  void Build(const vector<int>& a_) {
    n = a_.size();
    a = a_;
    Prepare();
    a.resize(n * 2, INT_MIN);
    sa.resize(n);
    rank.resize(n);
    vector<array<int, 2>> key(n);
    for (int i = 0; i < n; i++) key[i] = {a[i], -1};
    Resort(key);
    for (int len = 1; len <= n; len *= 2) {
      for (int i = 0; i < n; i++) {
        int r1 = rank[i];
        int r2 = i + len < n ? rank[i + len] : -1;
        key[i] = {r1, r2};
      }
      Resort(key);
    }
    height.resize(n);
    for (int i = 0, p = 0; i < n; i++) {
      if (rank[i] == 0) continue;
      int j = sa[rank[i] - 1];
      while (a[i + p] == a[j + p]) p++;
      height[rank[i]] = p;
      if (p) p--;
    }
  }
};

struct SparseTable {
  int n;
  vector<int> log;
  vector<vector<int>> f;

  void Build(const vector<int>& a) {
    n = a.size();
    log.resize(n + 1);
    log[1] = 0;
    for (int i = 2, x = 0; i <= n; i++) {
      if (i == (i & -i)) {
        log[i] = log[i - 1] + 1;
      } else {
        log[i] = log[i - 1];
      }
    }
    f.resize(n);
    for (int i = 0; i < n; i++) {
      f[i].resize(log[n] + 1);
      f[i][0] = a[i];
    }
    for (int j = 1; j <= log[n]; j++) {
      for (int i = 0; i + (1 << (j - 1)) < n; i++) {
        f[i][j] = min(f[i][j - 1], f[i + (1 << (j - 1))][j - 1]);
      }
    }
  }

  int Query(int l, int r) {
    int len = r - l + 1;
    int t = log[len];
    return min(f[l][t], f[r - (1 << t) + 1][t]);
  }
};

// string hash

const vector<pair<int, int>> hash_config = {{29, 1000000007}, {31, 1000000009}};

vector<vector<int>> fact;

struct HashString {
  vector<vector<int>> hash;

  HashString(const string& s) {
    hash.resize(hash_config.size());
    for (int i = 0; i < hash.size(); i++) {
      hash[i].resize(s.length());
      auto [p, mod] = hash_config[i];
      hash[i][0] = s[0] - 'a' + 1;
      for (int j = 1; j < s.length(); j++) {
        hash[i][j] = (int64)hash[i][j - 1] * p % mod;
        hash[i][j] = (hash[i][j] + s[j] - 'a' + 1) % mod;
      }
    }
  }

  vector<int> GetHash(int l, int r) const {
    assert(0 <= l);
    assert(l <= r);
    assert(r < hash[0].size());
    vector<int> res(hash_config.size());
    for (int i = 0; i < hash_config.size(); i++) {
      int mod = hash_config[i].second;
      int a = hash[i][r];
      int b = (int64)(l == 0 ? 0 : hash[i][l - 1]) * fact[i][r - l + 1] % mod;
      res[i] = (a - b + mod) % mod;
    }
    return res;
  }

  int Length() const {
    return hash[0].size();
  }

  static void Prepare(int n) {
    fact.resize(hash_config.size());
    for (int i = 0; i < fact.size(); i++) {
      fact[i].resize(n + 1);
      auto [p, mod] = hash_config[i];
      fact[i][0] = 1;
      for (int j = 1; j <= n; j++) {
        fact[i][j] = (int64)fact[i][j - 1] * p % mod;
      }
    }
  }
};

// KMP

struct KMP {
  string s;
  vector<int> next;

  KMP(const string& s_) : s(s_) {
    next.resize(s.length());
    std::fill(next.begin(), next.end(), 0);
    for (int i = 1, j = 0; i < s.length(); i++) {
      while (j && s[j] != s[i]) j = next[j - 1];
      if (s[j] == s[i]) j++;
      next[i] = j;
    }
  }

  vector<int> Match(const string& t) {
    vector<int> res;
    for (int i = 0, j = 0; i < t.length(); i++) {
      while (j && s[j] != t[i]) j = next[j - 1];
      if (s[j] == t[i]) j++;
      if (j == s.length()) {
        res.push_back(i - s.length() + 1);
        j = next[j - 1];
      }
    }
    return res;
  }
};

void Main() {}

int main() {
#ifdef LOCAL
  freopen("../input.txt", "r", stdin);
  // freopen("../output.txt", "w", stdout);
#endif
  Main();
  return 0;
}
