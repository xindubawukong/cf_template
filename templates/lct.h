#ifndef LINK_CUT_TREE_H_
#define LINK_CUT_TREE_H_

#include <vector>

#include "splay.h"

/*
struct Info {
  SplayTree<Info>::Node* Node() {
    return reinterpret_cast<SplayTree<Info>::Node*>(this);
  }
  bool rev;
  int id, faid;
  Info() : rev(false), faid(-1) {}
  void Reverse() {
    auto x = Node();
    swap(x->lch, x->rch);
    rev ^= 1;
  }
  bool NeedPushDown() { return rev; }
  void PushDown() {
    auto lch = Node()->lch, rch = Node()->rch;
    if (lch) lch->info.Reverse();
    if (rch) rch->info.Reverse();
    rev = false;
  }
  void Update() {}
};
*/

template <typename Info>
struct LCT {
  using Node = typename SplayTree<Info>::Node;
  int n;
  std::vector<Node*> node;
  SplayTree<Info> splay;
  LCT(int n_) : n(0) { ExpandTo(n_); }
  void ExpandTo(int m) {
    if (m <= n) return;
    node.resize(m);
    for (int i = n; i < m; i++) {
      Info info;
      info.id = i;
      node[i] = new Node(info);
    }
    n = m;
  }
  void Splay(Node* x) {
    auto y = x;
    while (y->fa) y = y->fa;
    splay.Splay(x);
    swap(x->info.faid, y->info.faid);
  }
  void Splay(int x) { Splay(node[x]); }
  void Access(Node* x) {
    for (Node* y = nullptr; x;
         y = x, x = x->info.faid == -1 ? nullptr : node[x->info.faid]) {
      Splay(x);
      splay.PushDown(x);
      if (x->rch) {
        x->rch->fa = nullptr;
        x->rch->info.faid = x->info.id;
      }
      x->rch = y;
      if (y) {
        y->fa = x;
        y->info.faid = -1;
      }
      splay.Update(x);
    }
  }
  void Access(int x) { Access(node[x]); }
  Node* GetRoot(Node* x) {
    Access(x);
    Splay(x);
    while (x->lch) {
      splay.PushDown(x);
      x = x->lch;
    }
    Splay(x);
    return x;
  }
  int GetRoot(int x) { return GetRoot(node[x])->info.id; }
  void MakeRoot(Node* x) {
    Access(x);
    Splay(x);
    x->info.Reverse();
  }
  void MakeRoot(int x) { MakeRoot(node[x]); }
  void Link(Node* x, Node* y) {
    MakeRoot(x);
    x->info.faid = y->info.id;
  }
  void Link(int x, int y) { Link(node[x], node[y]); }
  void Cut(Node* x, Node* y) {
    MakeRoot(x);
    Access(y);
    Splay(y);
    x->fa = y->lch = nullptr;
    x->info.faid = -1;
  }
  void Cut(int x, int y) { Cut(node[x], node[y]); }
};

#endif  // LINK_CUT_TREE_H_
