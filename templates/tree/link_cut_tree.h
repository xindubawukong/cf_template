#ifndef LINK_CUT_TREE_H_
#define LINK_CUT_TREE_H_

#include <vector>

#include "ds/splay.h"

/*
struct Info {
  Info() {}
  void Reverse() {}
  bool NeedPushDown() { return false; }
  void PushDown() {}
  void Update(Info* a, Info* b) {}
};
*/

template <typename Info>
struct LinkCutTree {
  struct SplayInfo : public Info {
    SplayTree<SplayInfo>::Node* Node() {
      return reinterpret_cast<SplayTree<SplayInfo>::Node*>(this);
    }
    bool rev;
    int id, upid;
    SplayInfo() : rev(false), upid(-1) {}
    void Reverse() {
      auto x = Node();
      std::swap(x->lch, x->rch);
      rev ^= 1;
      Info::Reverse();
    }
    bool NeedPushDown() { return rev || Info::NeedPushDown(); }
    void PushDown() {
      auto lch = Node()->lch, rch = Node()->rch;
      if (rev) {
        if (lch) lch->Reverse();
        if (rch) rch->Reverse();
        rev = false;
      }
      if (Info::NeedPushDown()) {
        Info::PushDown(lch ? (Info*)lch : nullptr, rch ? (Info*)rch : nullptr);
      }
    }
    void Update() {
      auto lch = Node()->lch, rch = Node()->rch;
      Info::Update(lch ? (Info*)lch : nullptr, rch ? (Info*)rch : nullptr);
    }
  };

  using Node = typename SplayTree<SplayInfo>::Node;

  int n;
  std::vector<Node*> node;
  SplayTree<SplayInfo> splay;
  LinkCutTree(int n_) : n(0) { ExpandTo(n_); }
  void ExpandTo(int m) {
    if (m <= n) return;
    node.resize(m);
    for (int i = n; i < m; i++) {
      SplayInfo info;
      info.id = i;
      node[i] = new Node(info);
    }
    n = m;
  }
  void Splay(Node* x) {
    auto y = x;
    while (y->fa) y = y->fa;
    splay.Splay(x);
    std::swap(x->upid, y->upid);
    x->Update();
  }
  void Splay(int x) { Splay(node[x]); }
  void Access(Node* x) {
    for (Node* y = nullptr; x;
         y = x, x = x->upid == -1 ? nullptr : node[x->upid]) {
      Splay(x);
      splay.PushDown(x);
      if (x->rch) x->rch->fa = nullptr, x->rch->upid = x->id;
      x->rch = y;
      if (y) y->fa = x, y->upid = -1;
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
  int GetRoot(int x) { return GetRoot(node[x])->id; }
  void MakeRoot(Node* x) {
    Access(x);
    Splay(x);
    x->Reverse();
  }
  void MakeRoot(int x) { MakeRoot(node[x]); }
  void Link(Node* x, Node* y) {
    MakeRoot(x);
    x->upid = y->id;
  }
  void Link(int x, int y) { Link(node[x], node[y]); }
  void Cut(Node* x, Node* y) {
    MakeRoot(x);
    Access(y);
    Splay(y);
    x->fa = y->lch = nullptr;
    y->Update();
    x->upid = -1;
  }
  void Cut(int x, int y) { Cut(node[x], node[y]); }
};

#endif  // LINK_CUT_TREE_H_
