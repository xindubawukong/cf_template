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
  auto Node() { return reinterpret_cast<Treap<Info>::Node*>(this); }
  Info() {}
  bool NeedPushDown() { return false; }
  void PushDown() {}
  void Update() {}
};
*/

template <bool>
struct TreapNodeBase {};
template <>
struct TreapNodeBase<false> {};
template <>
struct TreapNodeBase<true> {
  int ts;
};

template <typename Info, bool persist = false>
struct Treap {
  using info_t = Info;

  struct Node : public Info, public TreapNodeBase<persist> {
    unsigned int priority;
    Node *lch, *rch;
    Node(int ts_ = 0) : lch(nullptr), rch(nullptr) {
      static std::mt19937 rng(0);
      priority = rng();
      if constexpr (persist) this->ts = ts_;
    }
    // when copying a node, must be in persist mode
    Node(Node* x, int ts) {
      *this = *x;
      if constexpr (persist) this->ts = ts;
    }
    void Set(const Info& info) { (Info&)(*this) = info; }
  };

  Node* Create() { return new Node(ts); }
  Node* Copy(Node* x) { return new Node(x, ts); }

  Node* root;
  int ts;  // plus version if persistence is wanted
  Treap() : root(nullptr), ts(0) {}

  Node* Update(Node* x) {
    x->Update();
    return x;
  }
  void PushDown(Node* x) {
    if (x->NeedPushDown()) {
      if constexpr (persist) {
        if (x->lch && x->lch->ts != ts) x->lch = new Node(x->lch, ts);
        if (x->rch && x->rch->ts != ts) x->rch = new Node(x->rch, ts);
      }
      x->PushDown();
    }
  }

  Node* Join(Node* x, Node* y) {
    if (!x) return y ? Update(y) : nullptr;
    if (!y) return Update(x);
    if constexpr (persist) {
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
    if (!x) return Join(y, z);
    if (!y) return Join(x, z);
    if (!z) return Join(x, y);
    assert(!y->lch && !y->rch);  // y must be single node
    if constexpr (persist) {
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
    if (!x) return {nullptr, nullptr, nullptr};
    if constexpr (persist) {
      if (x->ts != ts) x = new Node(x, ts);
    }
    PushDown(x);
    auto d = cmp(x);
    if (d == 0) {
      auto l = x->lch, r = x->rch;
      if constexpr (persist) {
        if (l && l->ts != ts) l = new Node(l, ts);
        if (r && r->ts != ts) r = new Node(r, ts);
      }
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

  void GC(Node* x) {
    if (!x) return;
    GC(x->lch);
    GC(x->rch);
    delete x;
  }
};

#endif  // TREAP_H_