#ifndef SPLAY_H_
#define SPLAY_H_

#include <cassert>

/*
struct Info {
  SplayTree<Info>::Node* Node() {
    return reinterpret_cast<SplayTree<Info>::Node*>(this);
  }
  Info() {}
  bool NeedPushDown() { return false; }
  void PushDown() {}
  void Update() {}
};
*/

// clang-format off
template <typename Info>
struct SplayTree {
  using info_t = Info;
  struct Node {
    Info info;
    Node *lch, *rch, *fa;
    Node(Info info_) :lch(nullptr),rch(nullptr), fa(nullptr), info(info_){};
  };
  Node* root;
  SplayTree() : root(nullptr) {}
  Node* Update(Node* x) {
    x->info.Update();
    return x;
  }
  void PushDown(Node* x) {
    if (x->info.NeedPushDown()) {
      x->info.PushDown();
    }
  }
  // t == y->lch
  void Zig(Node* t) {
    auto y = t->fa;
    PushDown(y), PushDown(t);
    t->fa = y->fa;
    if (y->fa) {
      if (y == y->fa->lch) y->fa->lch = t;
      else y->fa->rch = t;
    }
    y->lch = t->rch;
    if (t->rch) t->rch->fa = y;
    t->rch = y;
    y->fa = t;
    Update(y), Update(t);
  }
  // t == y->rch
  void Zag(Node* t) {
    auto y = t->fa;
    PushDown(y), PushDown(t);
    t->fa = y->fa;
    if (y->fa) {
      if (y == y->fa->lch) y->fa->lch = t;
      else y->fa->rch = t;
    }
    y->rch = t->lch;
    if (t->lch) t->lch->fa = y;
    t->lch = y;
    y->fa = t;
    Update(y), Update(t);
  }
  // splay t under p
  void Splay(Node* t, Node* p = nullptr) {
    assert(t);
    while (t->fa != p) {
      auto y = t->fa;
      if (y->fa == p) {
        if (t == y->lch) Zig(t);
        else Zag(t);
      } else {
        if (y == y->fa->lch) {
          if (t == y->lch) Zig(y), Zig(t);
          else Zag(t), Zig(t);
        } else {
          if (t == y->rch) Zag(y), Zag(t);
          else Zig(t), Zag(t);
        }
      }
    }
    if (!p) root = t;
  }
};
// clang-format on

#endif  // SPLAY_H_
