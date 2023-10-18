#ifndef MATCHING_H_
#define MATCHING_H_

#include <cassert>
#include <vector>

struct Matching {
  int n, m, res = 0, ts = 0;
  std::vector<std::vector<int>> go;
  std::vector<int> vt, pa, pb;
  Matching(int n_, int m_) : n(n_), m(m_) {
    assert(n > 0 && m > 0);
    go.resize(n);
    vt.resize(n, -1);
    pa.resize(n, -1);
    pb.resize(m, -1);
  }
  void AddEdge(int u, int v) {
    assert(u >= 0 && u < n && v >= 0 && v < m);
    go[u].push_back(v);
  }
  bool Dfs(int u) {
    vt[u] = ts;
    for (int v : go[u]) {
      if (pb[v] == -1) {
        pa[u] = v;
        pb[v] = u;
        return true;
      }
    }
    for (int v : go[u]) {
      if (vt[pb[v]] != ts && Dfs(pb[v])) {
        pa[u] = v;
        pb[v] = u;
        return true;
      }
    }
    return false;
  }
  int Solve() {
    for (int i = 0; i < n; i++) {
      if (pa[i] == -1) {
        ts++;
        if (Dfs(i)) res++;
      }
    }
    return res;
  }
};

struct HopcroftKarp {
  int n, m, res = 0;
  std::vector<std::vector<int>> go;
  std::vector<int> d1, d2, pa, pb;
  std::vector<int> que;
  HopcroftKarp(int n_, int m_) : n(n_), m(m_) {
    assert(n > 0 && m > 0);
    go.resize(n);
    d1.resize(n);
    d2.resize(m);
    pa.resize(n, -1);
    pb.resize(m, -1);
  }
  void AddEdge(int u, int v) {
    assert(u >= 0 && u < n && v >= 0 && v < m);
    go[u].push_back(v);
  }
  bool Bfs() {
    std::fill(d1.begin(), d1.end(), -1);
    std::fill(d2.begin(), d2.end(), -1);
    que.clear();
    for (int i = 0; i < n; i++) {
      if (pa[i] == -1) {
        d1[i] = 0;
        que.push_back(i);
      }
    }
    bool flag = false;
    for (int i = 0; i < (int)que.size(); i++) {
      int u = que[i];
      for (int v : go[u]) {
        if (d2[v] != -1) continue;
        d2[v] = d1[u] + 1;
        if (pb[v] == -1) flag = true;
        else {
          d1[pb[v]] = d2[v] + 1;
          que.push_back(pb[v]);
        }
      }
    }
    return flag;
  }
  bool Dfs(int u) {
    for (int v : go[u]) {
      if (d2[v] == d1[u] + 1) {
        d2[v] = -1;
        if (pb[v] == -1 || Dfs(pb[v])) {
          pb[v] = u;
          pa[u] = v;
          return true;
        }
      }
    }
    return false;
  }
  int Solve() {
    int ans = 0;
    while (Bfs()) {
      for (int i = 0; i < n; i++) {
        if (pa[i] == -1) ans += Dfs(i);
      }
    }
    return ans;
  }
};

#endif  // MATCHING_H_
