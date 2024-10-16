#ifndef TWO_SAT_H_
#define TWO_SAT_H_

#include <cassert>
#include <vector>

#include "graph.h"
#include "tarjan_directed.h"

struct TwoSat {
  int n;
  DirectedGraph<EdgeBase> g;
  bool ok;
  std::vector<bool> ans;

  TwoSat(int n_) : n(n_), g(2 * n_) {}

  void Add(int i, bool x, int j, bool y) {
    debug(i, x, j, y);
    assert(0 <= i && i < n && 0 <= j && j < n);
    g.AddEdge({i * 2 + !x, j * 2 + y});
    g.AddEdge({j * 2 + !y, i * 2 + x});
  }

  void Solve() {
    TarjanDirected tj(g);
    auto& belong = tj.belong;
    ok = true;
    ans.resize(n);
    for (int i = 0; i < n; i++) {
      if (belong[i * 2] == belong[i * 2 + 1]) {
        ok = false;
      } else if (belong[i * 2] < belong[i * 2 + 1]) {
        ans[i] = false;
      } else {
        ans[i] = true;
      }
    }
  }
};

#endif  // TWO_SAT_H_
