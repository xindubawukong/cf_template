#ifndef AHO_CORASICK_AUTOMATON_H_
#define AHO_CORASICK_AUTOMATON_H_

#include <array>
#include <queue>
#include <string>
#include <vector>

#include "graph/graph.h"

template <char offset = 'a', int K = 26>
struct AhoCorasickAutomaton {
  int tot;
  std::vector<std::array<int, K>> son;
  std::vector<int> fail;
  AhoCorasickAutomaton() : tot(1) { Expand(); }
  int Insert(const std::string& s) {
    int now = 0;
    for (int i = 0; i < s.length(); i++) {
      int c = s[i] - offset;
      if (!son[now][c]) {
        Expand();
        son[now][c] = tot++;
      }
      now = son[now][c];
    }
    return now;
  }
  void Build() {
    std::queue<int> que;
    for (int i = 0; i < K; i++) {
      if (son[0][i]) que.push(son[0][i]);
    }
    while (!que.empty()) {
      int x = que.front();
      que.pop();
      for (int i = 0; i < K; i++) {
        if (son[x][i]) {
          fail[son[x][i]] = son[fail[x]][i];
          que.push(son[x][i]);
        } else {
          son[x][i] = son[fail[x]][i];
        }
      }
    }
  }
  DirectedGraph<EdgeBase> FailTree() {
    DirectedGraph<EdgeBase> g(tot);
    for (int i = 1; i < tot; i++) {
      g.AddEdge({fail[i], i});
    }
    return g;
  }

 private:
  void Expand() {
    son.push_back({});
    fail.push_back(0);
  }
};

#endif  // AHO_CORASICK_AUTOMATON_H_
