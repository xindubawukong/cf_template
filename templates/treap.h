#ifndef TREAP_H_
#define TREAP_H_

#include <cassert>
#include <functional>
#include <random>
#include <tuple>
#include <vector>

#include "bst_utils.h"

/*
struct Info {
  Treap<Info>::Node* Node() {
    return reinterpret_cast<Treap<Info>::Node*>(this);
  }
  Info() {}
  bool NeedPushDown() { return false; }
  void PushDown() {}
  void Update() {}
};
*/

template <typename Info>
struct Treap {
  using info_t = Info;
  struct Node {
    Info info;
    unsigned int priority;
    int ts;
    Node *lch, *rch;
    Node(Info info_, int ts_ = 0)
        : ts(ts_), lch(nullptr), rch(nullptr), info(info_) {
      static std::mt19937 rng(0);
      priority = rng();
    }
    // when copying a node, must be in persist mode
    Node(Node* x, int ts_)
        : priority(x->priority),
          ts(ts_),
          lch(x->lch),
          rch(x->rch),
          info(x->info) {}
  };

  Node* root;
  bool persist;
  int ts;  // plus version if persistence is wanted
  Treap(bool persist_ = false) : root(nullptr), persist(persist_), ts(0) {}

  Node* Update(Node* x) {
    x->info.Update();
    return x;
  }

  void PushDown(Node* x) {
    if (x->info.NeedPushDown()) {
      if (persist) {
        if (x->lch && x->lch->ts != ts) x->lch = new Node(x->lch, ts);
        if (x->rch && x->rch->ts != ts) x->rch = new Node(x->rch, ts);
      }
      x->info.PushDown();
    }
  }

  Node* Join(Node* x, Node* y) {
    if (x == nullptr) return y;
    if (y == nullptr) return x;
    if (persist) {
      if (x->ts != ts) x = new Node(x, ts);
      if (y->ts != ts) y = new Node(y, ts);
    }
    PushDown(x);
    PushDown(y);
    if (x->priority >= y->priority) {
      x->rch = Join(x->rch, y);
      return Update(x);
    } else {
      y->lch = Join(x, y->lch);
      return Update(y);
    }
  }

  Node* Join(Node* x, Node* y, Node* z) {
    if (x == nullptr) return Join(y, z);
    if (y == nullptr) return Join(x, z);
    if (z == nullptr) return Join(x, y);
    assert(y->lch == nullptr && y->rch == nullptr);  // y must be single node
    if (persist) {
      if (x->ts != ts) x = new Node(x, ts);
      if (y->ts != ts) y = new Node(y, ts);
      if (z->ts != ts) z = new Node(z, ts);
    }
    PushDown(x);
    PushDown(z);
    if (x->priority >= y->priority && x->priority >= z->priority) {
      x->rch = Join(x->rch, y, z);
      return Update(x);
    } else if (y->priority >= z->priority) {
      y->lch = x;
      y->rch = z;
      return Update(y);
    } else {
      z->lch = Join(x, y, z->lch);
      return Update(z);
    }
  }

  // cmp(info) < 0: split lch
  // cmp(info) = 0: return this
  // cmp(info) > 0: split rch
  template <typename Cmp>
  std::tuple<Node*, Node*, Node*> Split(Node* x, Cmp cmp) {
    if (x == nullptr) {
      return {nullptr, nullptr, nullptr};
    }
    if (persist) {
      if (x->ts != ts) x = new Node(x, ts);
    }
    PushDown(x);
    auto d = cmp(x->info);
    if (d == 0) {
      auto l = x->lch, r = x->rch;
      if (l && l->ts != ts) l = new Node(l, ts);
      if (r && r->ts != ts) r = new Node(r, ts);
      x->lch = x->rch = nullptr;
      return {l, Update(x), r};
    } else if (d < 0) {
      auto [w, y, z] = Split(x->lch, cmp);
      x->lch = nullptr;
      return {w, y, Join(z, Update(x))};
    } else {
      auto [w, y, z] = Split(x->rch, cmp);
      x->rch = nullptr;
      return {Join(Update(x), w), y, z};
    }
  }

  auto SplitKth(Node* x, int k) { return Split(x, bst::KthCmp<Info>(k)()); }
};

#endif  // TREAP_H_