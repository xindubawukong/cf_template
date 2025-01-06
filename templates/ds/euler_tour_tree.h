#ifndef EULER_TOUR_TREE_H_
#define EULER_TOUR_TREE_H_

#include <map>
#include <vector>

#include "splay.h"

struct EulerTourTree {
  struct Info {
    int u, v;
    int64_t val, sum;
    SplayTree<Info>::Node* Node() {
      return reinterpret_cast<SplayTree<Info>::Node*>(this);
    }
    Info(int u_, int v_) : u(u_), v(v_), val(0), sum(0) {}
    bool NeedPushDown() { return false; }
    void PushDown() {}
    void Update() {
      auto lch = Node()->lch, rch = Node()->rch;
      sum = val;
      if (lch) sum += lch->sum;
      if (rch) sum += rch->sum;
    }
  };

  using Node = SplayTree<Info>::Node;

  Node* CreateNode(int u, int v) { return new Node(Info(u, v)); }
  void Splay(Node* x, Node* y = nullptr) { splay.Splay(x, y); }
  void PushDown(Node* x) { splay.PushDown(x); }
  void Update(Node* x) { splay.Update(x); }

  std::vector<Node*> vertices;
  std::map<std::pair<int, int>, Node*> edges;
  SplayTree<Info> splay;
  EulerTourTree(int n) {
    vertices.resize(n);
    for (int i = 0; i < n; ++i) {
      vertices[i] = CreateNode(i, i);
    }
  }

  void MoveToFirst(Node* x) {
    Splay(x);
    PushDown(x);
    if (!x->lch) return;
    if (!x->rch) {
      std::swap(x->lch, x->rch);
      Update(x);
      return;
    }
    Node* y = x->rch;
    for (;;) {
      PushDown(y);
      if (!y->rch) break;
      y = y->rch;
    }
    Splay(y, x);
    std::swap(x->lch, y->rch);
    y->rch->fa = y;
    Update(y);
    Update(x);
  }

  void Link(int u, int v) {
    Node* x = vertices[u];
    Node* y = vertices[v];
    MoveToFirst(x);
    MoveToFirst(y);
    assert(!x->fa);
    Node* uv = CreateNode(u, v);
    Node* vu = CreateNode(v, u);
    x->lch = vu;
    vu->fa = x;
    Update(x);
    uv->lch = x, uv->rch = y;
    x->fa = uv, y->fa = uv;
    Update(uv);
    edges[{u, v}] = uv;
    edges[{v, u}] = vu;
  }

  void Cut(int u, int v) {
    assert(edges.contains({u, v}) && edges.contains({v, u}));
    Node* uv = edges[{u, v}];
    Node* vu = edges[{v, u}];
    MoveToFirst(uv);
    Splay(vu, uv);
    vu->lch->fa = nullptr;
    vu->rch->fa = nullptr;
    delete uv;
    delete vu;
    edges.erase({u, v});
    edges.erase({v, u});
  }
};

#endif  // EULER_TOUR_TREE_H_
